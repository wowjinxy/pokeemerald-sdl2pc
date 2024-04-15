#include "global.h"
#include "gpu_main.h"

struct GpuState gpu;

void GpuInit(void)
{
    gpu.gfxDataSize = 256 * 1000;
    gpu.tileMapsSize = 256 * 1000;
    gpu.spriteGfxDataSize = 256 * 1000;

    gpu.gfxData = calloc(1, gpu.gfxDataSize);
    if (!gpu.gfxData)
        abort();

    gpu.tileMaps = calloc(1, gpu.tileMapsSize);
    if (!gpu.tileMaps)
        abort();

    gpu.spriteGfxData = calloc(1, gpu.spriteGfxDataSize);
    if (!gpu.spriteGfxData)
        abort();
}

void GpuClearData(void)
{
    memset(gpu.gfxData, 0, gpu.gfxDataSize);
    memset(gpu.tileMaps, 0, gpu.tileMapsSize);
    memset(gpu.spriteGfxData, 0, gpu.spriteGfxDataSize);
}

void GpuClearSprites(void)
{
    memset(gpu.spriteList, 0, sizeof(gpu.spriteList));
}

void GpuClearPalette(void)
{
    memset(gpu.palette, 0, sizeof(gpu.palette));
}

void GpuClearPalette2(void)
{
    memset(gpu.palette + 2, 0, sizeof(gpu.palette) - 2);
}

void GpuClearAll(void)
{
    GpuClearData();
    GpuClearSprites();
    GpuClearPalette();
}
