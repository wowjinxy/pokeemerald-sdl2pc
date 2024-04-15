#ifndef GUARD_GBA_GPUSTATE_H
#define GUARD_GBA_GPUSTATE_H

#include "gba/types.h"
#include "gba/defines.h"

#define MAX_OAM_SPRITES 128

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
};

extern struct GpuState gpu;

#endif
