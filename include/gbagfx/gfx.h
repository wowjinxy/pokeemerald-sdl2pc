// Copyright (c) 2015 YamaArashi

#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <stdbool.h>

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};

struct Palette {
    struct Color colors[256];
    int numColors;
};

struct Image {
    int width;
    int height;
    int bitDepth;
    unsigned char *pixels;
    bool hasPalette;
    struct Palette palette;
    bool hasTransparency;
};

enum NumTilesMode {
    NUM_TILES_IGNORE,
    NUM_TILES_WARN,
    NUM_TILES_ERROR,
};

void *WriteTileImage(enum NumTilesMode numTilesMode, int numTiles, int metatileWidth, int metatileHeight, struct Image *image, bool invertColors, size_t *size);
void *WritePlainImage(int dataWidth, struct Image *image, bool invertColors, size_t *size);
void FreeImage(struct Image *image);
void *WriteGbaPalette(struct Palette *palette, size_t *size);

#endif // GFX_H
