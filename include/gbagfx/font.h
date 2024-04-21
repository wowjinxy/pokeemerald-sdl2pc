// Copyright (c) 2015 YamaArashi

#ifndef FONT_H
#define FONT_H

#include <stdbool.h>
#include "gfx.h"

void *WriteLatinFont(struct Image *image, size_t *size);
void *WriteHalfwidthJapaneseFont(struct Image *image, size_t *size);
void *WriteFullwidthJapaneseFont(struct Image *image, size_t *size);

#endif // FONT_H
