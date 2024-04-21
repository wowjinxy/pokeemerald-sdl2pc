// Copyright (c) 2015 YamaArashi

#include "global.h"
#include "gbagfx/gfx.h"
#include "util.h"

#define SET_GBA_PAL(r, g, b) (((b) << 10) | ((g) << 5) | (r))

#define DOWNCONVERT_BIT_DEPTH(x) ((x) / 8)

static void AdvanceMetatilePosition(int *subTileX, int *subTileY, int *metatileX, int *metatileY, int metatilesWide, int metatileWidth, int metatileHeight)
{
    (*subTileX)++;
    if (*subTileX == metatileWidth) {
        *subTileX = 0;
        (*subTileY)++;
        if (*subTileY == metatileHeight) {
            *subTileY = 0;
            (*metatileX)++;
            if (*metatileX == metatilesWide) {
                *metatileX = 0;
                (*metatileY)++;
            }
        }
    }
}

static void ConvertToTiles1Bpp(unsigned char *src, unsigned char *dest, int numTiles, int metatilesWide, int metatileWidth, int metatileHeight, bool invertColors)
{
    int subTileX = 0;
    int subTileY = 0;
    int metatileX = 0;
    int metatileY = 0;
    int pitch = metatilesWide * metatileWidth;

    for (int i = 0; i < numTiles; i++) {
        for (int j = 0; j < 8; j++) {
            int srcY = (metatileY * metatileHeight + subTileY) * 8 + j;
            int srcX = metatileX * metatileWidth + subTileX;
            unsigned char srcPixelOctet = src[srcY * pitch + srcX];
            unsigned char *destPixelOctet = dest++;

            for (int k = 0; k < 8; k++) {
                *destPixelOctet <<= 1;
                *destPixelOctet |= (srcPixelOctet & 1) ^ invertColors;
                srcPixelOctet >>= 1;
            }
        }

        AdvanceMetatilePosition(&subTileX, &subTileY, &metatileX, &metatileY, metatilesWide, metatileWidth, metatileHeight);
    }
}

static void ConvertToTiles4Bpp(unsigned char *src, unsigned char *dest, int numTiles, int metatilesWide, int metatileWidth, int metatileHeight, bool invertColors)
{
    int subTileX = 0;
    int subTileY = 0;
    int metatileX = 0;
    int metatileY = 0;
    int pitch = (metatilesWide * metatileWidth) * 4;

    for (int i = 0; i < numTiles; i++) {
        for (int j = 0; j < 8; j++) {
            int srcY = (metatileY * metatileHeight + subTileY) * 8 + j;

            for (int k = 0; k < 4; k++) {
                int srcX = (metatileX * metatileWidth + subTileX) * 4 + k;
                unsigned char srcPixelPair = src[srcY * pitch + srcX];
                unsigned char leftPixel = srcPixelPair >> 4;
                unsigned char rightPixel = srcPixelPair & 0xF;

                if (invertColors) {
                    leftPixel = 15 - leftPixel;
                    rightPixel = 15 - rightPixel;
                }

                *dest++ = (rightPixel << 4) | leftPixel;
            }
        }

        AdvanceMetatilePosition(&subTileX, &subTileY, &metatileX, &metatileY, metatilesWide, metatileWidth, metatileHeight);
    }
}

static void ConvertToTiles8Bpp(unsigned char *src, unsigned char *dest, int numTiles, int metatilesWide, int metatileWidth, int metatileHeight, bool invertColors)
{
    int subTileX = 0;
    int subTileY = 0;
    int metatileX = 0;
    int metatileY = 0;
    int pitch = (metatilesWide * metatileWidth) * 8;

    for (int i = 0; i < numTiles; i++) {
        for (int j = 0; j < 8; j++) {
            int srcY = (metatileY * metatileHeight + subTileY) * 8 + j;

            for (int k = 0; k < 8; k++) {
                int srcX = (metatileX * metatileWidth + subTileX) * 8 + k;
                unsigned char srcPixel = src[srcY * pitch + srcX];

                if (invertColors)
                    srcPixel = 255 - srcPixel;

                *dest++ = srcPixel;
            }
        }

        AdvanceMetatilePosition(&subTileX, &subTileY, &metatileX, &metatileY, metatilesWide, metatileWidth, metatileHeight);
    }
}

// For untiled, plain images
static void CopyPlainPixels(unsigned char *src, unsigned char *dest, int size, int dataWidth, bool invertColors)
{
    if (dataWidth == 0) return;
    for (int i = 0; i < size; i += dataWidth) {
        for (int j = dataWidth; j > 0; j--) {
            unsigned char pixels = src[i + j - 1];
            *dest++ = invertColors ? ~pixels : pixels;
        }
    }
}

void *WriteTileImage(enum NumTilesMode numTilesMode, int numTiles, int metatileWidth, int metatileHeight, struct Image *image, bool invertColors, size_t *size)
{
    int tileSize = image->bitDepth * 8;

    if (image->width % 8 != 0) {
        fprintf(stderr, "The width in pixels (%d) isn't a multiple of 8.\n", image->width);
        return NULL;
    }

    if (image->height % 8 != 0) {
        fprintf(stderr, "The height in pixels (%d) isn't a multiple of 8.\n", image->height);
        return NULL;
    }

    int tilesWidth = image->width / 8;
    int tilesHeight = image->height / 8;

    if (tilesWidth % metatileWidth != 0) {
        fprintf(stderr, "The width in tiles (%d) isn't a multiple of the specified metatile width (%d)\n", tilesWidth, metatileWidth);
        return NULL;
    }

    if (tilesHeight % metatileHeight != 0) {
        fprintf(stderr, "The height in tiles (%d) isn't a multiple of the specified metatile height (%d)\n", tilesHeight, metatileHeight);
        return NULL;
    }

    int maxNumTiles = tilesWidth * tilesHeight;

    if (numTiles == 0)
        numTiles = maxNumTiles;
    else if (numTiles > maxNumTiles) {
        fprintf(stderr, "The specified number of tiles (%d) is greater than the maximum possible value (%d).\n", numTiles, maxNumTiles);
        return NULL;
    }

    int bufferSize = numTiles * tileSize;
    int maxBufferSize = maxNumTiles * tileSize;
    unsigned char *buffer = malloc(maxBufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for pixels.\n");
        return NULL;
    }

    int metatilesWide = tilesWidth / metatileWidth;

    switch (image->bitDepth) {
    case 1:
        ConvertToTiles1Bpp(image->pixels, buffer, maxNumTiles, metatilesWide, metatileWidth, metatileHeight, invertColors);
        break;
    case 4:
        ConvertToTiles4Bpp(image->pixels, buffer, maxNumTiles, metatilesWide, metatileWidth, metatileHeight, invertColors);
        break;
    case 8:
        ConvertToTiles8Bpp(image->pixels, buffer, maxNumTiles, metatilesWide, metatileWidth, metatileHeight, invertColors);
        break;
    }

    bool zeroPadded = true;
    for (int i = bufferSize; i < maxBufferSize && zeroPadded; i++) {
        if (buffer[i] != 0)
        {
            switch (numTilesMode)
            {
            case NUM_TILES_IGNORE:
                break;
            case NUM_TILES_WARN:
                fprintf(stderr, "Ignoring -num_tiles %d because tile %d contains non-transparent pixels.\n", numTiles, 1 + i / tileSize);
                zeroPadded = false;
                break;
            case NUM_TILES_ERROR:
                fprintf(stderr, "Tile %d contains non-transparent pixels.\n", 1 + i / tileSize);
                break;
            }
        }
    }

    if (zeroPadded) {
        *size = (size_t)bufferSize;
        buffer = realloc(buffer, bufferSize);
    }
    else
        *size = (size_t)maxBufferSize;

    return buffer;
}

void *WritePlainImage(int dataWidth, struct Image *image, bool invertColors, size_t *size)
{
    int bufferSize = image->width * image->height * image->bitDepth / 8;

    if (bufferSize % dataWidth != 0) {
        fprintf(stderr, "The image data size (%d) isn't a multiple of the specified data width %d.\n", bufferSize, dataWidth);
        return NULL;
    }

    // png scanlines have wasted bits if they do not align to byte boundaries.
    // pngs misaligned in this way are not currently handled.
    int pixelsPerByte = 8 / image->bitDepth;
    if (image->width % pixelsPerByte != 0) {
        fprintf(stderr, "The width in pixels (%d) isn't a multiple of %d.\n", image->width, pixelsPerByte);
        return NULL;
    }

    unsigned char *buffer = malloc(bufferSize);

    if (buffer == NULL) {
        fprintf(stderr, "Failed to allocate memory for pixels.\n");
        return NULL;
    }

    *size = (size_t)bufferSize;

    CopyPlainPixels(image->pixels, buffer, bufferSize, dataWidth, invertColors);

    return buffer;
}

void FreeImage(struct Image *image)
{
    free(image->pixels);
    image->pixels = NULL;
}

void *WriteGbaPalette(struct Palette *palette, size_t *size)
{
    size_t paletteDataSize = palette->numColors * 2;
    uint16_t *data = malloc(paletteDataSize);
    if (data == NULL) {
        fprintf(stderr, "Could not allocate %d bytes of memory for WriteGbaPalette.\n", paletteDataSize);
        return NULL;
    }

    *size = paletteDataSize;

    for (int i = 0; i < palette->numColors; i++) {
        unsigned char red = DOWNCONVERT_BIT_DEPTH(palette->colors[i].red);
        unsigned char green = DOWNCONVERT_BIT_DEPTH(palette->colors[i].green);
        unsigned char blue = DOWNCONVERT_BIT_DEPTH(palette->colors[i].blue);

        data[i] = SET_GBA_PAL(red, green, blue);
    }

    return data;
}
