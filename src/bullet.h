#ifndef _BULLET_H
#define _BULLET_H

#include "defines.h"
#include "graphics.h"
#include "collision.h"

#define PLAYER_BULLET_TOTAL 6
#define SHRAPNEL_BULLET_TOTAL 12

#define PLAYER_BULLET_PER_PLAYER 3

#define PLAYER_BULLET_W 16
#define PLAYER_BULLET_H 8

#define SHRAPNEL_BULLET_SIZE 8

typedef struct {
    SDL_Rect rect, srcRect;
    u8 active;
    Hitbox hitbox;
} PlayerBullet;

typedef struct {
    SDL_Rect rect, srcRect;
    char dX, dY;
    u8 active, associatedPlayerIndex, multiplier;
    Hitbox hitbox;
} ShrapnelBullet;

extern PlayerBullet playerBullets[];
extern ShrapnelBullet shrapnelBullets[];

void PlayerBulletFireNext(u8, int, int);
void PlayerBulletUpdate(u8);
void PlayerBulletDraw(u8);
void PlayerBulletDeactivate(u8);

void ShrapnelBulletSpray(int, int, u8, u8);
void ShrapnelBulletUpdate(u8);
void ShrapnelBulletDraw(u8);
void ShrapnelBulletDeactivate(u8);

#endif
