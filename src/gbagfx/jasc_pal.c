// Copyright (c) 2015 YamaArashi

#include "global.h"
#include "gbagfx/gfx.h"
#include "util.h"

// Read/write Paint Shop Pro palette files.

// Format of a Paint Shop Pro palette file, line by line:
// "JASC-PAL\r\n" (signature)
// "0100\r\n" (version; seems to always be "0100")
// "<NUMBER_OF_COLORS>\r\n" (number of colors in decimal)
//
// <NUMBER_OF_COLORS> times:
// "<RED> <GREEN> <BLUE>\r\n" (color entry)
//
// Each color component is a decimal number from 0 to 255.
// Examples:
// Black - "0 0 0\r\n"
// Blue  - "0 0 255\r\n"
// Brown - "150 75 0\r\n"

#define MAX_LINE_LENGTH 11

bool ReadJascPaletteLine(FILE *fp, char *line)
{
    int c;
    int length = 0;

    for (;;)
    {
        c = fgetc(fp);

        if (c == '\r')
        {
            c = fgetc(fp);

            if (c != '\n') {
                fprintf(stderr, "CR line endings aren't supported.\n");
                return false;
            }

            line[length] = 0;

            return true;
        }

        if (c == '\n')
        {
            fprintf(stderr, "LF line endings aren't supported.\n");
            return false;
        }

        if (c == EOF) {
            fprintf(stderr, "Unexpected EOF. No CRLF at end of file.\n");
            return false;
        }

        if (c == 0) {
            fprintf(stderr, "NUL character in file.\n");
            return false;
        }

        if (length == MAX_LINE_LENGTH)
        {
            line[length] = 0;
            fprintf(stderr, "The line \"%s\" is too long.\n", line);
            return false;
        }

        line[length++] = c;
    }

    return true;
}

bool ReadJascPalette(const char *path, struct Palette *palette)
{
    char line[MAX_LINE_LENGTH + 1];

    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        fprintf(stderr, "Failed to open JASC-PAL file \"%s\" for reading.\n", path);
        return false;
    }

    if (!ReadJascPaletteLine(fp, line)) {
        fclose(fp);
        return false;
    }

    if (strcmp(line, "JASC-PAL") != 0) {
        fprintf(stderr, "Invalid JASC-PAL signature.\n");
        fclose(fp);
        return false;
    }

    if (!ReadJascPaletteLine(fp, line)) {
        fclose(fp);
        return false;
    }

    if (strcmp(line, "0100") != 0) {
        fprintf(stderr, "Unsuported JASC-PAL version.\n");
        fclose(fp);
        return false;
    }

    if (!ReadJascPaletteLine(fp, line)) {
        fclose(fp);
        return false;
    }

    if (!ParseNumber(line, NULL, 10, &palette->numColors)) {
        fprintf(stderr, "Failed to parse number of colors.\n");
        fclose(fp);
        return false;
    }

    if (palette->numColors < 1 || palette->numColors > 256) {
        fprintf(stderr, "%d is an invalid number of colors. The number of colors must be in the range [1, 256].\n", palette->numColors);
        fclose(fp);
        return false;
    }

    for (int i = 0; i < palette->numColors; i++)
    {
        if (!ReadJascPaletteLine(fp, line)) {
            fclose(fp);
            return false;
        }

        char *s = line;
        char *end;

        int red;
        int green;
        int blue;

        if (!ParseNumber(s, &end, 10, &red)) {
            fprintf(stderr, "Failed to parse red color component.\n");
            fclose(fp);
            return false;
        }

        s = end;

        if (*s != ' ') {
            fprintf(stderr, "Expected a space after red color component.\n");
            fclose(fp);
            return false;
        }

        s++;

        if (*s < '0' || *s > '9') {
            fprintf(stderr, "Expected only a space between red and green color components.\n");
            fclose(fp);
            return false;
        }

        if (!ParseNumber(s, &end, 10, &green)) {
            fprintf(stderr, "Failed to parse green color component.\n");
            fclose(fp);
            return false;
        }

        s = end;

        if (*s != ' ') {
            fprintf(stderr, "Expected a space after green color component.\n");
            fclose(fp);
            return false;
        }

        s++;

        if (*s < '0' || *s > '9') {
            fprintf(stderr, "Expected only a space between green and blue color components.\n");
            fclose(fp);
            return false;
        }

        if (!ParseNumber(s, &end, 10, &blue)) {
            fprintf(stderr, "Failed to parse blue color component.\n");
            fclose(fp);
            return false;
        }

        if (*end != 0) {
            fprintf(stderr, "Garbage after blue color component.\n");
            fclose(fp);
            return false;
        }

        if (red < 0 || red > 255) {
            fprintf(stderr, "Red color component (%d) is outside the range [0, 255].\n", red);
            fclose(fp);
            return false;
        }

        if (green < 0 || green > 255) {
            fprintf(stderr, "Green color component (%d) is outside the range [0, 255].\n", green);
            fclose(fp);
            return false;
        }

        if (blue < 0 || blue > 255) {
            fprintf(stderr, "Blue color component (%d) is outside the range [0, 255].\n", blue);
            fclose(fp);
            return false;
        }

        palette->colors[i].red = red;
        palette->colors[i].green = green;
        palette->colors[i].blue = blue;
    }

    if (fgetc(fp) != EOF) {
        fprintf(stderr, "Garbage after color data.\n");
        fclose(fp);
        return false;
    }

    fclose(fp);

    return true;
}
