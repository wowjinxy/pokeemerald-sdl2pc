#ifndef GUARD_GBA_GPUSTATE_H
#define GUARD_GBA_GPUSTATE_H

#include "gba/types.h"
#include "gba/defines.h"

#define MAX_OAM_SPRITES 128

#define NUM_CHAR_BLOCKS 4

#define NUM_SCREEN_BLOCKS 32

enum
{
    GPU_STATE_DISPCNT,
    GPU_STATE_DISPSTAT,
    GPU_STATE_VCOUNT,
    GPU_STATE_MOSAIC,
    GPU_STATE_BLDCNT,
    GPU_STATE_BLDALPHA,
    GPU_STATE_BLDY
};

struct GpuBgState
{
    s32 x, y;

    struct BgCnt control;
};

struct GpuAffineBgState
{
    s16 pa, pb, pc, pd;
    s32 x, y;
};

struct GpuWindowState
{
    u32 x, y;
};

struct GpuState
{
    u8 *gfxData;
    u8 *tileMaps;
    u8 *spriteGfxData;

    struct OamData spriteList[MAX_OAM_SPRITES];

    unsigned char palette[PLTT_SIZE];

    size_t gfxDataSize;
    size_t tileMapsSize;
    size_t spriteGfxDataSize;

    u32 displayControl;
    u32 displayStatus;
    u32 vCount;

    struct GpuBgState bg[NUM_BACKGROUNDS];

    struct GpuAffineBgState affineBg2;
    struct GpuAffineBgState affineBg3;

    struct {
        struct GpuWindowState state[2];
        u8 in, out;
    } window;

    u8 mosaic;
    u8 blendControl;
    u8 blendAlpha;
    u8 blendCoeff;
};

extern struct GpuState gpu;

#endif
