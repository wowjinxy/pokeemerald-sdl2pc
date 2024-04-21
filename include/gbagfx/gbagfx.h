// Copyright (c) 2015 YamaArashi

#ifndef GBAGFX_H
#define GBAGFX_H

#include <stdint.h>
#include <stdbool.h>

#include "options.h"
#include "gfx.h"

void *HandlePngToGbaCommand(const char *inputPath, struct PngToGbaOptions *options, size_t *outSize, void **outPalette, size_t *outPaletteSize);

void *HandleJascToGbaPaletteCommand(const char *inputPath, int numColors, size_t *size);

void *HandlePngToLatinFontCommand(const char *inputPath, size_t *size);
void *HandlePngToHalfwidthJapaneseFontCommand(const char *inputPath, size_t *size);
void *HandlePngToFullwidthJapaneseFontCommand(const char *inputPath, size_t *size);

void *HandleLZCompressCommand(const char *inputPath, int *compressedSize, int overflowSize, int minDistance);
void *HandleLZDecompressCommand(const char *inputPath, int *uncompressedSize);

void *HandleRLCompressCommand(const char *inputPath, int *compressedSize);
void HandleRLDecompressCommand(const char *inputPath, int *uncompressedSize);

void *HandleHuffCompressCommand(const char *inputPath, int *compressedSize, int bitDepth);
void *HandleHuffDecompressCommand(const char *inputPath, int *uncompressedSize);

#endif // GBAGFX_H
