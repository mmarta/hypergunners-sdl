#ifndef _PLAYER_H
#define _PLAYER_H

#include "defines.h"
#include "system.h"
#include "graphics.h"
#include "collision.h"
#include "bullet.h"
#include "multiple.h"
#include "clawline.h"

#define PLAYER_SIZE 16
#define PLAYER_TOTAL 2

#define PLAYER_BULLETS_COUNT 3

typedef struct {
    u8 active, lives, animTime, killTime;
    u8 clawPressed, shootPressed, controllable, index;
    u32 score;
    ClawLine clawLine;
    SDL_Rect rect, spriteRect;
    Hitbox hitbox;
    Multiple multiple;
    PlayerBullet bullets[PLAYER_BULLETS_COUNT];
} Player;

extern Player players[];

void PlayerInit(Player *, u8 i);
void PlayerInput(Player *);
void PlayerMove(Player *, int, int);
void PlayerKill(Player *);
void PlayerUpdate(Player *);
void PlayerDraw(Player *);

#endif
