#ifndef GUARD_GPU_MAIN_H
#define GUARD_GPU_MAIN_H

void GpuInit(void);

void GpuClearData(void);
void GpuClearSprites(void);
void GpuClearPalette(void);
void GpuClearPalette2(void);
void GpuClearAll(void);

void *GpuGetGfxPtr(u8 bgNum);
void *GpuGetTilemapPtr(u8 bgNum);
void GpuClearTilemap(u8 bgNum);

#endif
