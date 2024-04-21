// Copyright (c) 2015 YamaArashi

#include "global.h"
#include "gbagfx/font.h"
#include "gbagfx/gfx.h"
#include "util.h"

static void ConvertToLatinFont(unsigned char *src, unsigned char *dest, unsigned int numRows)
{
    unsigned int destPixelsOffset = 0;

    for (unsigned int row = 0; row < numRows; row++) {
        for (unsigned int column = 0; column < 16; column++) {
            for (unsigned int glyphTile = 0; glyphTile < 4; glyphTile++) {
                unsigned int pixelsX = (column * 16) + ((glyphTile & 1) * 8);

                for (unsigned int i = 0; i < 8; i++) {
                    unsigned int pixelsY = (row * 16) + ((glyphTile >> 1) * 8) + i;
                    unsigned int srcPixelsOffset = (pixelsY * 64) + (pixelsX / 4);

                    dest[destPixelsOffset] = src[srcPixelsOffset + 1];
                    dest[destPixelsOffset + 1] = src[srcPixelsOffset];

                    destPixelsOffset += 2;
                }
            }
        }
    }
}

static void ConvertToHalfwidthJapaneseFont(unsigned char *src, unsigned char *dest, unsigned int numRows)
{
    for (unsigned int row = 0; row < numRows; row++) {
        for (unsigned int column = 0; column < 16; column++) {
            unsigned int glyphIndex = (row * 16) + column;

            for (unsigned int glyphTile = 0; glyphTile < 2; glyphTile++) {
                unsigned int pixelsX = column * 8;
                unsigned int destPixelsOffset = 512 * (glyphIndex >> 4) + 16 * (glyphIndex & 0xF) + 256 * glyphTile;

                for (unsigned int i = 0; i < 8; i++) {
                    unsigned int pixelsY = (row * 16) + (glyphTile * 8) + i;
                    unsigned int srcPixelsOffset = (pixelsY * 32) + (pixelsX / 4);

                    dest[destPixelsOffset] = src[srcPixelsOffset + 1];
                    dest[destPixelsOffset + 1] = src[srcPixelsOffset];

                    destPixelsOffset += 2;
                }
            }
        }
    }
}

static void ConvertToFullwidthJapaneseFont(unsigned char *src, unsigned char *dest, unsigned int numRows)
{
    for (unsigned int row = 0; row < numRows; row++) {
        for (unsigned int column = 0; column < 16; column++) {
            unsigned int glyphIndex = (row * 16) + column;

            for (unsigned int glyphTile = 0; glyphTile < 4; glyphTile++) {
                unsigned int pixelsX = (column * 16) + ((glyphTile & 1) * 8);
                unsigned int destPixelsOffset = 512 * (glyphIndex >> 3) + 32 * (glyphIndex & 7) + 256 * (glyphTile >> 1) + 16 * (glyphTile & 1);

                for (unsigned int i = 0; i < 8; i++) {
                    unsigned int pixelsY = (row * 16) + ((glyphTile >> 1) * 8) + i;
                    unsigned int srcPixelsOffset = (pixelsY * 64) + (pixelsX / 4);

                    dest[destPixelsOffset] = src[srcPixelsOffset + 1];
                    dest[destPixelsOffset + 1] = src[srcPixelsOffset];

                    destPixelsOffset += 2;
                }
            }
        }
    }
}

void *WriteLatinFont(struct Image *image, size_t *size)
{
    if (image->width != 256) {
        fprintf(stderr, "The width of the font image (%d) is not 256.\n", image->width);
        return NULL;
    }

    if (image->height % 16 != 0) {
        fprintf(stderr, "The height of the font image (%d) is not a multiple of 16.\n", image->height);
        return NULL;
    }

    int numRows = image->height / 16;
    int bufferSize = numRows * 16 * 64;
    unsigned char *buffer = malloc(bufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for font.\n");
        return NULL;
    }

    *size = (size_t)bufferSize;

    ConvertToLatinFont(image->pixels, buffer, numRows);

    return buffer;
}

void *WriteHalfwidthJapaneseFont(struct Image *image, size_t *size)
{
    if (image->width != 128) {
        fprintf(stderr, "The width of the font image (%d) is not 128.\n", image->width);
        return NULL;
    }

    if (image->height % 16 != 0) {
        fprintf(stderr, "The height of the font image (%d) is not a multiple of 16.\n", image->height);
        return NULL;
    }

    int numRows = image->height / 16;
    int bufferSize = numRows * 16 * 32;
    unsigned char *buffer = malloc(bufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for font.\n");
        return NULL;
    }

    *size = (size_t)bufferSize;

    ConvertToHalfwidthJapaneseFont(image->pixels, buffer, numRows);

    return buffer;
}

void *WriteFullwidthJapaneseFont(struct Image *image, size_t *size)
{
    if (image->width != 256) {
        fprintf(stderr, "The width of the font image (%d) is not 256.\n", image->width);
        return NULL;
    }

    if (image->height % 16 != 0) {
        fprintf(stderr, "The height of the font image (%d) is not a multiple of 16.\n", image->height);
        return NULL;
    }

    int numRows = image->height / 16;
    int bufferSize = numRows * 16 * 64;
    unsigned char *buffer = malloc(bufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for font.\n");
        return NULL;
    }

    *size = (size_t)bufferSize;

    ConvertToFullwidthJapaneseFont(image->pixels, buffer, numRows);

    return buffer;
}
