// Copyright (c) 2015 YamaArashi

#include <stdio.h>
#include <setjmp.h>
#include <png.h>
#include <stdlib.h>
#include "gbagfx/convert_png.h"
#include "gbagfx/gfx.h"

static FILE *PngReadOpen(char *path, png_structp *pngStruct, png_infop *pngInfo)
{
    FILE *fp = fopen(path, "rb");

    if (fp == NULL) {
        fprintf(stderr, "Failed to open \"%s\" for reading.\n", path);
        return NULL;
    }

    unsigned char sig[8];

    if (fread(sig, 8, 1, fp) != 1) {
        fprintf(stderr, "Failed to read PNG signature from \"%s\".\n", path);
        fclose(fp);
        return NULL;
    }

    if (png_sig_cmp(sig, 0, 8)) {
        fprintf(stderr, "\"%s\" does not have a valid PNG signature.\n", path);
        fclose(fp);
        return NULL;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        fprintf(stderr, "Failed to create PNG read struct.\n");
        fclose(fp);
        return NULL;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr) {
        fprintf(stderr, "Failed to create PNG info struct.\n");
        fclose(fp);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Failed to init I/O for reading \"%s\".\n", path);
        fclose(fp);
        return NULL;
    }

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);

    *pngStruct = png_ptr;
    *pngInfo = info_ptr;

    return fp;
}

static unsigned char *ConvertBitDepth(unsigned char *src, int srcBitDepth, int destBitDepth, int numPixels)
{
    // Round the number of bits up to the next 8 and divide by 8 to get the number of bytes.
    int srcSize = ((numPixels * srcBitDepth + 7) & ~7) / 8;
    int destSize = ((numPixels * destBitDepth + 7) & ~7) / 8;
    unsigned char *output = calloc(destSize, 1);
    unsigned char *dest = output;
    int i;
    int j;
    int destBit = 8 - destBitDepth;

    for (i = 0; i < srcSize; i++)
    {
        unsigned char srcByte = src[i];

        for (j = 8 - srcBitDepth; j >= 0; j -= srcBitDepth)
        {
            unsigned char pixel = (srcByte >> j) % (1 << destBitDepth);
            *dest |= pixel << destBit;
            destBit -= destBitDepth;
            if (destBit < 0)
            {
                dest++;
                destBit = 8 - destBitDepth;
            }
        }
    }

    return output;
}

bool ReadPng(const char *path, struct Image *image, struct Palette *palette)
{
    png_structp png_ptr;
    png_infop info_ptr;

    FILE *fp = PngReadOpen(path, &png_ptr, &info_ptr);
    if (!fp)
        return false;

    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    int color_type = png_get_color_type(png_ptr, info_ptr);

    if (color_type != PNG_COLOR_TYPE_GRAY && color_type != PNG_COLOR_TYPE_PALETTE) {
        fprintf(stderr, "\"%s\" has an unsupported color type.\n", path);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    // Check if the image has a palette so that we can tell if the colors need to be inverted later.
    image->hasPalette = color_type == PNG_COLOR_TYPE_PALETTE;

    image->width = png_get_image_width(png_ptr, info_ptr);
    image->height = png_get_image_height(png_ptr, info_ptr);

    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    image->pixels = malloc(image->height * rowbytes);

    if (image->pixels == NULL) {
        fprintf(stderr, "Failed to allocate pixel buffer.\n");
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    png_bytepp row_pointers = malloc(image->height * sizeof(png_bytep));

    if (row_pointers == NULL) {
        fprintf(stderr, "Failed to allocate row pointers.\n");
        FreeImage(image);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    for (int i = 0; i < image->height; i++)
        row_pointers[i] = (png_bytep)(image->pixels + (i * rowbytes));

    if (setjmp(png_jmpbuf(png_ptr))) {
        fprintf(stderr, "Error reading from \"%s\".\n", path);
        FreeImage(image);
        free(row_pointers);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return false;
    }

    png_read_image(png_ptr, row_pointers);

    if (palette && image->hasPalette) {
        png_colorp colors;
        int numColors;

        if (png_get_PLTE(png_ptr, info_ptr, &colors, &numColors) != PNG_INFO_PLTE) {
            fprintf(stderr, "Failed to retrieve palette from \"%s\".\n", path);
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
        }

        if (numColors > 256) {
            fprintf(stderr, "Images with more than 256 colors are not supported.\n");
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            fclose(fp);
            return false;
        }

        palette->numColors = numColors;
        for (int i = 0; i < numColors; i++) {
            palette->colors[i].red = colors[i].red;
            palette->colors[i].green = colors[i].green;
            palette->colors[i].blue = colors[i].blue;
        }
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    free(row_pointers);
    fclose(fp);

    if (bit_depth != image->bitDepth)
    {
        unsigned char *src = image->pixels;

        if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8) {
            fprintf(stderr, "Bit depth of image must be 1, 2, 4, or 8.\n");
            FreeImage(image);
            return false;
        }

        image->pixels = ConvertBitDepth(src, bit_depth, image->bitDepth, image->width * image->height);
        free(src);
    }

    return true;
}
