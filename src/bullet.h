#ifndef _BULLET_H
#define _BULLET_H

#include "defines.h"
#include "graphics.h"
#include "collision.h"

#define PLAYER_BULLET_TOTAL 6
#define SHRAPNEL_BULLET_TOTAL 12
#define MULTIPLE_BULLET_TOTAL 12

#define PLAYER_BULLET_PER_PLAYER 3
#define MULTIPLE_BULLET_PER_PLAYER 6

#define PLAYER_BULLET_W 16
#define PLAYER_BULLET_H 8

#define SHRAPNEL_BULLET_SIZE 8

typedef enum {
    STANDARD_PLAYER_BULLET_TYPE,
    DOUBLE_PLAYER_BULLET_TYPE
} PlayerBulletType;

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
} EnemyBullet;

// Enemy bullet pools
extern EnemyBullet shrapnelBullets[];

void PlayerBulletFireNext(PlayerBullet [], int, int, u8);
void PlayerBulletUpdate(PlayerBullet *);
void PlayerBulletDraw(PlayerBullet *);
void PlayerBulletDeactivate(PlayerBullet *);

void MultipleBulletFireNext(PlayerBullet [], int, int, PlayerBulletType, u8, u8);

void ShrapnelBulletSpray(int, int, u8, u8);
void ShrapnelBulletUpdate(EnemyBullet *);
void ShrapnelBulletDraw(EnemyBullet *);
void ShrapnelBulletDeactivate(EnemyBullet *);

#endif
