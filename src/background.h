#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "defines.h"
#include "graphics.h"

#define STAR_TOTAL 50

#define BACKGROUND_LINE_R 0
#define BACKGROUND_LINE_G 0
#define BACKGROUND_LINE_B 255

void BackgroundInit();
void BackgroundUpdate();
void BackgroundDraw();

#endif
