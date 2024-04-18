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

void SetGpuState(u8 state, u32 val);
u32 GetGpuState(u8 state);

void SetGpuStateBits(u8 state, u32 mask);
void ClearGpuStateBits(u8 state, u32 mask);

void SetGpuBackgroundX(u8 bgNum, u32 x);
void SetGpuBackgroundY(u8 bgNum, u32 y);

u32 GetGpuBackgroundX(u8 bgNum);
u32 GetGpuBackgroundY(u8 bgNum);

void SetGpuWindowX(u8 windowNum, u32 x);
void SetGpuWindowY(u8 windowNum, u32 y);
void SetGpuWindowIn(u32 in);
void SetGpuWindowOut(u32 out);

u32 GetGpuWindowX(u8 windowNum);
u32 GetGpuWindowY(u8 windowNum);
u32 GetGpuWindowIn(void);
u32 GetGpuWindowOut(void);

void SetGpuAffineBgA(u8 bgNum, u32 a);
void SetGpuAffineBgB(u8 bgNum, u32 b);
void SetGpuAffineBgC(u8 bgNum, u32 c);
void SetGpuAffineBgD(u8 bgNum, u32 d);
void SetGpuAffineBgX(u8 bgNum, u32 x);
void SetGpuAffineBgY(u8 bgNum, u32 y);

u32 GetGpuAffineBgA(u8 bgNum);
u32 GetGpuAffineBgB(u8 bgNum);
u32 GetGpuAffineBgC(u8 bgNum);
u32 GetGpuAffineBgD(u8 bgNum);
u32 GetGpuAffineBgX(u8 bgNum);
u32 GetGpuAffineBgY(u8 bgNum);

void SetGpuBackgroundPriority(u8 bgNum, u32 priority);
void SetGpuBackgroundCharBaseBlock(u8 bgNum, u32 charBaseBlock);
void SetGpuBackgroundMosaicEnabled(u8 bgNum, u32 mosaic);
void SetGpuBackground8bppMode(u8 bgNum, u32 use8bpp);
void SetGpuBackgroundGbaMode(u8 bgNum, u32 use8bpp);
void SetGpuBackgroundScreenBaseBlock(u8 bgNum, u32 screenBaseBlock);
void SetGpuBackgroundAreaOverflowMode(u8 bgNum, u32 areaOverflowMode);
void SetGpuBackgroundWidth(u8 bgNum, u32 width);
void SetGpuBackgroundHeight(u8 bgNum, u32 height);

u32 GetGpuBackgroundPriority(u8 bgNum);
u32 GetGpuBackgroundCharBaseBlock(u8 bgNum);
u32 GetGpuBackgroundMosaicEnabled(u8 bgNum);
u32 GetGpuBackground8bppMode(u8 bgNum);
u32 GetGpuBackgroundGbaMode(u8 bgNum);
u32 GetGpuBackgroundScreenBaseBlock(u8 bgNum);
u32 GetGpuBackgroundAreaOverflowMode(u8 bgNum);
u32 GetGpuBackgroundWidth(u8 bgNum);
u32 GetGpuBackgroundHeight(u8 bgNum);

void ClearGpuBackgroundState(u8 bgNum);

void SetGpuScanlineEffect(u8 type, u8 param, u32 *src);
void ClearGpuScanlineEffect(void);
void GpuRefreshScanlineEffect(void);

#endif
