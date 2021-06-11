#ifndef _ENEMY_H
#define _ENEMY_H

#include "defines.h"
#include "graphics.h"
#include "collision.h"
#include "bullet.h"
#include "clawline.h"
#include "player.h"

#define ENEMY_SIZE 16
#define ENEMY_TOTAL 30

#define ENEMY_TYPE_STANDARD 0
#define ENEMY_TYPE_SHRAPNEL 1

typedef struct {
    SDL_Rect rect, srcRect;
    u16 score;
    Hitbox hitbox;
    char dirDelta;
    u8 active, animTime, killTime, multiplier, type;
    Player *clawLinedPlayer;
} Enemy;

extern Enemy enemies[];

void EnemyInitAll();
void EnemyKill(u8, Player *, u8);
void EnemyDeactivate(u8);
void EnemyGrab(u8, Player *);
void EnemyUpdate(u8);
void EnemyDraw(u8);
void EnemySpawnNext();

#endif
