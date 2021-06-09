#ifndef _CLAWLINE_H
#define _CLAWLINE_H

#include "defines.h"
#include "graphics.h"
#include "collision.h"

#define CLAWLINE_SIZE 16
#define CLAWLINE_START_Y 224

typedef struct {
    SDL_Rect rect, srcRect;
    u8 active, returning, grabbed;
    Hitbox hitbox;
} ClawLine;

void ClawLineFire(ClawLine *, u8, u8);
void ClawLineReturn(ClawLine *, u8);
void ClawLineUpdate(ClawLine *);
void ClawLineDraw(ClawLine *);
void ClawLineDeactivate(ClawLine *);

#endif
