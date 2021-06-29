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

typedef enum {
    STANDARD_ENEMY_TYPE,
    SHRAPNEL_ENEMY_TYPE
} EnemyType;

typedef struct {
    SDL_Rect rect, srcRect;
    u16 score;
    Hitbox hitbox;
    char dirDelta;
    u8 active, animTime, killTime, multiplier;
    EnemyType type;
    Player *clawLinedPlayer;
} Enemy;

extern Enemy enemies[];

void EnemyInitAll();
void EnemyKill(Enemy *, Player *, u8);
void EnemyDeactivate(Enemy *);
void EnemyGrab(Enemy *, Player *);
void EnemyUpdate(Enemy *);
void EnemyDraw(Enemy *);
void EnemySpawnNext();

#endif
