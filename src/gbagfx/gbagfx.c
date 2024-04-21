// Copyright (c) 2015 YamaArashi

#include "global.h"
#include "util.h"
#include "gbagfx/options.h"
#include "gbagfx/gfx.h"
#include "gbagfx/convert_png.h"
#include "gbagfx/jasc_pal.h"
#include "gbagfx/lz.h"
#include "gbagfx/rl.h"
#include "gbagfx/font.h"
#include "gbagfx/huff.h"

static void *ConvertPngToGba(const char *inputPath, struct PngToGbaOptions *options, struct Palette *palette, size_t *size)
{
    struct Image image;

    void *result;

    image.bitDepth = options->bitDepth;

    if (!ReadPng(inputPath, &image, palette))
        return NULL;

    if (!options->isPlainImage)
        result = WriteTileImage(options->numTilesMode, options->numTiles, options->metatileWidth, options->metatileHeight, &image, !image.hasPalette, size);
    else
        result = WritePlainImage(options->dataWidth, &image, !image.hasPalette, size);

    FreeImage(&image);

    return result;
}

void *HandlePngToGbaCommand(const char *inputPath, struct PngToGbaOptions *options, size_t *outSize, void **outPalette, size_t *outPaletteSize)
{
    if (options->numTiles < 0) {
        fprintf(stderr, "Number of tiles must be positive.\n");
        return NULL;
    }

    if (options->metatileWidth == 0)
        options->metatileWidth = 1;
    else if (options->metatileWidth < 1) {
        fprintf(stderr, "Metatile width must be positive.\n");
        return NULL;
    }

    if (options->metatileHeight == 0)
        options->metatileHeight = 1;
    else if (options->metatileHeight < 1) {
        fprintf(stderr, "Metatile height must be positive.\n");
        return NULL;
    }

    if (options->dataWidth == 0)
        options->dataWidth = 1;
    else if (options->dataWidth < 1) {
        fprintf(stderr, "Data width must be positive.\n");
        return NULL;
    }

    if (options->bitDepth == 0)
        options->bitDepth = 4;

    struct Palette palette = {};
    void *result = ConvertPngToGba(inputPath, options, outPalette ? &palette : NULL, outSize);
    if (result) {
        if (outPalette)
            *outPalette = WriteGbaPalette(&palette, outPaletteSize);
        return result;
    }

    return NULL;
}

void *HandleJascToGbaPaletteCommand(const char *inputPath, int numColors, size_t *size)
{
    if (numColors < 0) {
        fprintf(stderr, "Number of colors must be positive.\n");
        return NULL;
    }

    struct Palette palette = {};

    if (!ReadJascPalette(inputPath, &palette))
        return NULL;

    if (numColors != 0)
        palette.numColors = numColors;

    return WriteGbaPalette(&palette, size);
}

void *HandlePngToLatinFontCommand(const char *inputPath, size_t *size)
{
    struct Image image;

    image.bitDepth = 2;

    if (!ReadPng(inputPath, &image, NULL))
        return NULL;

    void *result = WriteLatinFont(&image, size);

    FreeImage(&image);

    return result;
}

void *HandlePngToHalfwidthJapaneseFontCommand(const char *inputPath, size_t *size)
{
    struct Image image;

    image.bitDepth = 2;

    if (!ReadPng(inputPath, &image, NULL))
        return NULL;

    void *result = WriteHalfwidthJapaneseFont(&image, size);

    FreeImage(&image);

    return result;
}

void *HandlePngToFullwidthJapaneseFontCommand(const char *inputPath, size_t *size)
{
    struct Image image;

    image.bitDepth = 2;

    if (!ReadPng(inputPath, &image, NULL))
        return NULL;

    void *result = WriteFullwidthJapaneseFont(&image, size);

    FreeImage(&image);

    return result;
}

void *HandleLZCompressCommand(const char *inputPath, int *compressedSize, int overflowSize, int minDistance)
{
    if (overflowSize < 0) {
        fprintf(stderr, "Overflow size must be zero or positive.\n");
        return NULL;
    }

    if (minDistance < 1)
        minDistance = 2; // default, for compatibility with LZ77UnCompVram()

    // The overflow option allows a quirk in some of Ruby/Sapphire's tilesets
    // to be reproduced. It works by appending a number of zeros to the data
    // before compressing it and then amending the LZ header's size field to
    // reflect the expected size. This will cause an overflow when decompressing
    // the data.

    int fileSize;
    unsigned char *buffer = ReadWholeFileZeroPadded(inputPath, &fileSize, overflowSize);
    if (!buffer)
        return NULL;

    unsigned char *compressedData = LZCompress(buffer, fileSize + overflowSize, compressedSize, minDistance);
    if (compressedData) {
        compressedData[1] = (unsigned char)fileSize;
        compressedData[2] = (unsigned char)(fileSize >> 8);
        compressedData[3] = (unsigned char)(fileSize >> 16);
    }

    free(buffer);

    return compressedData;
}

void *HandleLZDecompressCommand(const char *inputPath, int *uncompressedSize)
{
    int fileSize;
    unsigned char *buffer = ReadWholeFile(inputPath, &fileSize);
    if (!buffer)
        return NULL;

    unsigned char *uncompressedData = LZDecompress(buffer, fileSize, uncompressedSize);

    free(buffer);

    return uncompressedData;
}

void *HandleRLCompressCommand(const char *inputPath, int *compressedSize)
{
    int fileSize;
    unsigned char *buffer = ReadWholeFile(inputPath, &fileSize);
    if (!buffer)
        return NULL;

    unsigned char *compressedData = RLCompress(buffer, fileSize, compressedSize);

    free(buffer);

    return compressedData;
}

void *HandleRLDecompressCommand(const char *inputPath, int *uncompressedSize)
{
    int fileSize;
    unsigned char *buffer = ReadWholeFile(inputPath, &fileSize);
    if (!buffer)
        return NULL;

    unsigned char *uncompressedData = RLDecompress(buffer, fileSize, uncompressedSize);

    free(buffer);

    return uncompressedData;
}

void *HandleHuffCompressCommand(const char *inputPath, int *compressedSize, int bitDepth)
{
    int fileSize;

    if (bitDepth == 0)
        bitDepth = 4;
    else if (bitDepth != 4 && bitDepth != 8) {
        fprintf(stderr, "GBA only supports bit depth of 4 or 8.\n");
        return false;
    }

    unsigned char *buffer = ReadWholeFile(inputPath, &fileSize);
    if (!buffer)
        return NULL;

    unsigned char *compressedData = HuffCompress(buffer, fileSize, compressedSize, bitDepth);

    free(buffer);

    return compressedData;
}

void *HandleHuffDecompressCommand(const char *inputPath, int *uncompressedSize)
{
    int fileSize;
    unsigned char *buffer = ReadWholeFile(inputPath, &fileSize);
    if (!buffer)
        return NULL;

    unsigned char *uncompressedData = HuffDecompress(buffer, fileSize, uncompressedSize);

    free(buffer);

    return uncompressedData;
}
