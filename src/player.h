#ifndef _PLAYER_H
#define _PLAYER_H

#include "defines.h"
#include "system.h"
#include "graphics.h"
#include "collision.h"
#include "bullet.h"

#define PLAYER_SIZE 16
#define PLAYER_TOTAL 2

typedef struct {
    u8 active, lives, animTime, killTime, shootPressed, controllable, index;
    u32 score;
    SDL_Rect rect, spriteRect;
    Hitbox hitbox;
} Player;

extern Player players[];

void PlayerInit(u8);
void PlayerInput(u8);
void PlayerMove(u8, int, int);
void PlayerKill(u8);
void PlayerUpdate(u8);
void PlayerDraw(u8);

#endif
