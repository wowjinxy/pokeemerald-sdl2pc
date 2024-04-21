// Copyright (c) 2018 huderlem

#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>
#include "gfx.h"

struct PngToGbaOptions {
    int numTiles;
    enum NumTilesMode numTilesMode;
    int bitDepth;
    int metatileWidth;
    int metatileHeight;
    bool isPlainImage;
    int dataWidth;
};

#endif // OPTIONS_H
