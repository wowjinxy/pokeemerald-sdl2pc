#ifndef GUARD_RESOURCES_LOADER_H
#define GUARD_RESOURCES_LOADER_H

#include "global.h"

bool8 LoadDataFromFile(struct DataSource *src, void *dest);

bool8 LoadImage(struct GraphicsSource *gfx, void *dest);
bool8 LoadImage8bpp(struct GraphicsSource *gfx, void *dest);

bool8 LoadImageAndPalette(struct GraphicsSource *gfx, void *dest, size_t palDest);
bool8 LoadImageAndPalette8bpp(struct GraphicsSource *gfx, void *dest, size_t palDest);

bool8 LoadImageAndPaletteColors(struct GraphicsSource *gfx, void *dest, size_t palDest, unsigned maxColors);
bool8 LoadImageAndPaletteColors8bpp(struct GraphicsSource *gfx, void *dest, size_t palDest, unsigned maxColors);

bool8 LoadPaletteFromFile(struct DataSource *src, size_t dest, int numColors);

void FreeLoadedAssets(void);

#endif // GUARD_RESOURCES_LOADER_H
