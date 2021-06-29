#ifndef _MULTIPLE_H
#define _MULTIPLE_H

#include "defines.h"
#include "graphics.h"
#include "collision.h"
#include "bullet.h"

#define MULTIPLE_TRAIL_X_TOTAL 8
#define MULTIPLE_BULLETS_COUNT 6
#define MULTIPLE_BULLETS_ROUND_COUNT MULTIPLE_BULLETS_COUNT >> 1

#define MULTIPLE_SIZE 16

typedef enum {
    STANDARD_MULTIPLE_TYPE,
    SPREAD_MULTIPLE_TYPE
} MultipleType;

typedef struct {
    SDL_Rect rect, srcRect;
    u8 trailX[MULTIPLE_TRAIL_X_TOTAL];
    u8 active, animTime;
    MultipleType type;
    PlayerBullet bullets[MULTIPLE_BULLETS_COUNT];
} Multiple;

void MultipleInit(Multiple *, u8, u8, MultipleType);
void MultipleFire(Multiple *);
void MultipleMoveX(Multiple *, u8);
void MultipleUpdate(Multiple *);
void MultipleDraw(Multiple *);
void MultipleDeactivate(Multiple *);

#endif
