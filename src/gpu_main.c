#include "global.h"
#include "gpu_main.h"

struct GpuState gpu;

void GpuInit(void)
{
    gpu.gfxDataSize = BG_CHAR_SIZE * NUM_CHAR_BLOCKS;
    gpu.tileMapsSize = BG_SCREEN_SIZE * NUM_SCREEN_BLOCKS;
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
    size_t offset;

    if (bgNum >= NUM_BACKGROUNDS)
        return NULL;

    offset = gpu.bg[bgNum].charBaseBlock % NUM_CHAR_BLOCKS;

    return gpu.gfxData + (offset * BG_CHAR_SIZE);
}

void *GpuGetTilemapPtr(u8 bgNum)
{
    size_t offset;

    if (bgNum >= NUM_BACKGROUNDS)
        return NULL;

    offset = gpu.bg[bgNum].screenBaseBlock % NUM_SCREEN_BLOCKS;

    return gpu.tileMaps + (offset * BG_SCREEN_SIZE);
}

void GpuClearTilemap(u8 bgNum)
{
    void *ptr = GpuGetTilemapPtr(bgNum);
    if (ptr)
        memset(ptr, 0, BG_SCREEN_SIZE);
}

void SetGpuState(u8 state, u32 val)
{
    switch (state)
    {
    case GPU_STATE_DISPCNT:
        gpu.displayControl = val;
        break;
    case GPU_STATE_DISPSTAT:
        gpu.displayStatus = val;
        break;
    case GPU_STATE_VCOUNT:
        gpu.vCount = val;
        break;
    case GPU_STATE_MOSAIC:
        gpu.mosaic = val;
        break;
    case GPU_STATE_BLDCNT:
        gpu.blendControl = val;
        break;
    case GPU_STATE_BLDALPHA:
        gpu.blendAlpha = val;
        break;
    case GPU_STATE_BLDY:
        gpu.blendCoeff = val;
        break;
    }
}

u32 GetGpuState(u8 state)
{
    switch (state)
    {
    case GPU_STATE_DISPCNT:
        return gpu.displayControl;
    case GPU_STATE_DISPSTAT:
        return gpu.displayStatus;
    case GPU_STATE_VCOUNT:
        return gpu.vCount;
    case GPU_STATE_MOSAIC:
        return gpu.mosaic;
    case GPU_STATE_BLDCNT:
        return gpu.blendControl;
    case GPU_STATE_BLDALPHA:
        return gpu.blendAlpha;
    case GPU_STATE_BLDY:
        return gpu.blendCoeff;
    }

    return 0;
}

void SetGpuBackgroundX(u8 bgNum, u32 x)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].x = x;
}

void SetGpuBackgroundY(u8 bgNum, u32 y)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].y = y;
}

void SetGpuWindowX(u8 windowNum, u32 x)
{
    if (windowNum > 1)
        return;

    gpu.window.state[windowNum].x = x;
}

void SetGpuWindowY(u8 windowNum, u32 y)
{
    if (windowNum > 1)
        return;

    gpu.window.state[windowNum].y = y;
}

void SetGpuWindowIn(u32 in)
{
    gpu.window.in = in;
}

void SetGpuWindowOut(u32 out)
{
    gpu.window.out = out;
}

u32 GetGpuBackgroundX(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].x;
}

u32 GetGpuBackgroundY(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].y;
}

u32 GetGpuWindowX(u8 windowNum)
{
    if (windowNum > 1)
        return 0;

    return gpu.window.state[windowNum].x;
}

u32 GetGpuWindowY(u8 windowNum)
{
    if (windowNum > 1)
        return 0;

    return gpu.window.state[windowNum].y;
}

u32 GetGpuWindowIn(void)
{
    return gpu.window.in;
}

u32 GetGpuWindowOut(void)
{
    return gpu.window.out;
}

static struct GpuAffineBgState *GetAffineBgState(u8 bgNum)
{
    switch (bgNum)
    {
    case 2:
        return &gpu.affineBg2;
    case 3:
        return &gpu.affineBg3;
    default:
        return NULL;
    }
}

void SetGpuAffineBgA(u8 bgNum, u32 a)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->pa = a;
}

void SetGpuAffineBgB(u8 bgNum, u32 b)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->pb = b;
}

void SetGpuAffineBgC(u8 bgNum, u32 c)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->pc = c;
}

void SetGpuAffineBgD(u8 bgNum, u32 d)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->pd = d;
}

void SetGpuAffineBgX(u8 bgNum, u32 x)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->x = x;
}

void SetGpuAffineBgY(u8 bgNum, u32 y)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return;

    state->y = y;
}

u32 GetGpuAffineBgA(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->pa;
}

u32 GetGpuAffineBgB(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->pb;
}

u32 GetGpuAffineBgC(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->pc;
}

u32 GetGpuAffineBgD(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->pd;
}

u32 GetGpuAffineBgX(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->x;
}

u32 GetGpuAffineBgY(u8 bgNum)
{
    struct GpuAffineBgState *state = GetAffineBgState(bgNum);
    if (state == NULL)
        return 0;

    return state->y;
}

void SetGpuBackgroundPriority(u8 bgNum, u32 priority)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].priority = priority;
}

void SetGpuBackgroundCharBaseBlock(u8 bgNum, u32 charBaseBlock)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].charBaseBlock = charBaseBlock;
}

void SetGpuBackgroundMosaicEnabled(u8 bgNum, u32 mosaic)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].mosaic = mosaic;
}

void SetGpuBackground8bppMode(u8 bgNum, u32 use8bpp)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].palettes = use8bpp;
}

void SetGpuBackgroundGbaMode(u8 bgNum, u32 gbaMode)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].gbaMode = gbaMode;
}

void SetGpuBackgroundScreenBaseBlock(u8 bgNum, u32 screenBaseBlock)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].screenBaseBlock = screenBaseBlock;
}

void SetGpuBackgroundAreaOverflowMode(u8 bgNum, u32 areaOverflowMode)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].areaOverflowMode = areaOverflowMode;
}

void SetGpuBackgroundWidth(u8 bgNum, u32 width)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].screenWidth = width;
}

void SetGpuBackgroundHeight(u8 bgNum, u32 height)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    gpu.bg[bgNum].screenHeight = height;
}

u32 GetGpuBackgroundPriority(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].priority;
}

u32 GetGpuBackgroundCharBaseBlock(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].charBaseBlock;
}

u32 GetGpuBackgroundMosaicEnabled(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].mosaic;
}

u32 GetGpuBackground8bppMode(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].palettes;
}

u32 GetGpuBackgroundGbaMode(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].gbaMode;
}

u32 GetGpuBackgroundScreenBaseBlock(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].screenBaseBlock;
}

u32 GetGpuBackgroundAreaOverflowMode(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].areaOverflowMode;
}

u32 GetGpuBackgroundWidth(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].screenWidth;
}

u32 GetGpuBackgroundHeight(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return 0;

    return gpu.bg[bgNum].screenHeight;
}

void ClearGpuBackgroundState(u8 bgNum)
{
    if (bgNum >= NUM_BACKGROUNDS)
        return;

    SetGpuBackgroundPriority(bgNum, 0);
    SetGpuBackgroundCharBaseBlock(bgNum, 0);
    SetGpuBackgroundMosaicEnabled(bgNum, 0);
    SetGpuBackground8bppMode(bgNum, 0);
    SetGpuBackgroundGbaMode(bgNum, 1);
    SetGpuBackgroundScreenBaseBlock(bgNum, 0);
    SetGpuBackgroundAreaOverflowMode(bgNum, 0);
    SetGpuBackgroundWidth(bgNum, 256);
    SetGpuBackgroundHeight(bgNum, 256);
}
