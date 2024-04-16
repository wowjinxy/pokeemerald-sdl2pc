#include "global.h"
#include "gpu_main.h"

struct GpuState gpu;

void GpuInit(void)
{
    gpu.gfxDataSize = BG_CHAR_SIZE * 4;
    gpu.tileMapsSize = BG_SCREEN_SIZE * 32;
    gpu.spriteGfxDataSize = 32000;

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

void *GpuGetGfxPtr(u8 bgNum)
{
    u16 *bgcnt = NULL;

    size_t offset;

    switch (bgNum)
    {
    case 0:
        bgcnt = (u16 *)REG_ADDR_BG0CNT;
        break;
    case 1:
        bgcnt = (u16 *)REG_ADDR_BG1CNT;
        break;
    case 2:
        bgcnt = (u16 *)REG_ADDR_BG2CNT;
        break;
    case 3:
        bgcnt = (u16 *)REG_ADDR_BG3CNT;
        break;
    default:
        return NULL;
    }

    offset = ((*bgcnt) >> 2) & 3;

    return gpu.gfxData + (offset * BG_CHAR_SIZE);
}

void *GpuGetTilemapPtr(u8 bgNum)
{
    u16 *bgcnt = NULL;

    size_t offset;

    switch (bgNum)
    {
    case 0:
        bgcnt = (u16 *)REG_ADDR_BG0CNT;
        break;
    case 1:
        bgcnt = (u16 *)REG_ADDR_BG1CNT;
        break;
    case 2:
        bgcnt = (u16 *)REG_ADDR_BG2CNT;
        break;
    case 3:
        bgcnt = (u16 *)REG_ADDR_BG3CNT;
        break;
    default:
        return NULL;
    }

    offset = ((*bgcnt) >> 8) & 0x1F;

    return gpu.tileMaps + (offset * BG_SCREEN_SIZE);
}

void GpuClearTilemap(u8 bgNum)
{
    void *ptr = GpuGetTilemapPtr(bgNum);
    if (ptr)
        CpuFill32(0, ptr, BG_SCREEN_SIZE);
}
