#include "enemy.h"

void EnemyInit(Enemy *);
void EnemyDrawUFO(Enemy *, u8);
void EnemyDrawStandard(Enemy *);
void EnemyDrawShrapnel(Enemy *);

Enemy enemies[ENEMY_TOTAL];

void EnemyInitAll() {
    u8 i = 0;
    while(i < ENEMY_TOTAL) {
        EnemyInit(&enemies[i]);
        i++;
    }
}

void EnemyInit(Enemy *enemy) {
    enemy->rect.w = ENEMY_SIZE;
    enemy->rect.h = ENEMY_SIZE;
    enemy->srcRect.w = ENEMY_SIZE;
    enemy->srcRect.h = ENEMY_SIZE;
    enemy->active = 0;
    enemy->killTime = 0;
    enemy->clawLinedPlayer = NULL;
}

void EnemyKill(Enemy *enemy, Player *player, u8 multiplier) {
    if(!multiplier) {
        multiplier = 1;
    }

    enemy->killTime = 1;
    enemy->multiplier = multiplier;
    enemy->hitbox.collidable = 0;
    if(enemy->type == SHRAPNEL_ENEMY_TYPE) {
        ShrapnelBulletSpray(enemy->rect.x, enemy->rect.y, player->index, multiplier == 32 ? multiplier : multiplier * 2);
    }
    player->score += (enemy->score * enemy->multiplier);
}

void EnemyDeactivate(Enemy *enemy) {
    enemy->killTime = 0;
    enemy->hitbox.collidable = 0;
    enemy->clawLinedPlayer = NULL;
    enemy->active = 0;
}

void EnemyGrab(Enemy *enemy, Player *player) {
    enemy->hitbox.collidable = 0;
    enemy->rect.x = player->clawLine.rect.x;
    enemy->rect.y = player->clawLine.rect.y;
    enemy->clawLinedPlayer = player;
}

void EnemyUpdate(Enemy *enemy) {
    if(!enemy->active) {
        return;
    }

    if(enemy->killTime) {
        enemy->killTime++;
        if(enemy->killTime >= 14) {
            EnemyDeactivate(enemy);
        }
        return;
    }

    enemy->animTime++;
    if(enemy->animTime >= 18) {
        enemy->animTime = 0;
    }

    // Movement
    if(enemy->clawLinedPlayer) {
        if(!enemy->clawLinedPlayer->clawLine.active) {
            enemy->clawLinedPlayer->score += (enemy->score << 1);
            MultipleInit(
                &enemy->clawLinedPlayer->multiple,
                enemy->clawLinedPlayer->rect.x,
                enemy->clawLinedPlayer->rect.y,
                enemy->type == SHRAPNEL_ENEMY_TYPE
                    ? SPREAD_MULTIPLE_TYPE : STANDARD_MULTIPLE_TYPE
            );
            EnemyDeactivate(enemy);

            return;
        }

        enemy->rect.x = enemy->clawLinedPlayer->clawLine.rect.x;
        enemy->rect.y = enemy->clawLinedPlayer->clawLine.rect.y;
        return;
    }

    if(enemy->rect.y < FAR_Y) {
        if(!(enemy->animTime % 2)) {
            enemy->rect.y++;
            enemy->hitbox.rect.y++;
        }
        if(enemy->rect.y >= FAR_Y) {
            enemy->hitbox.rect.x -= 2;
            enemy->hitbox.rect.w += 4;
            enemy->hitbox.rect.y -= 2;
            enemy->hitbox.rect.h++;
        }
    } else if(enemy->rect.y < MIDDLE_Y) {
        enemy->rect.y++;
        enemy->hitbox.rect.y++;
        if(enemy->rect.y >= MIDDLE_Y) {
            enemy->hitbox.rect.x -= 2;
            enemy->hitbox.rect.w += 4;
            enemy->hitbox.rect.y -= 3;
            enemy->hitbox.rect.h++;
        }
    } else {
        enemy->rect.y++;
        enemy->hitbox.rect.y++;
        if(enemy->rect.y >= 240) {
            enemy->hitbox.collidable = 0;
            enemy->active = 0;
        }
    }
}

void EnemyDrawUFO(Enemy *enemy, u8 srcY) {
    enemy->srcRect.y = srcY;

    if(enemy->rect.y < FAR_Y) {
        if(enemy->animTime % 6 < 3) {
            enemy->srcRect.x = 144;
        } else {
            enemy->srcRect.x = 160;
        }
    } else if(enemy->rect.y < MIDDLE_Y) {
        if(enemy->animTime % 9 < 3) {
            enemy->srcRect.x = 96;
        } else if(enemy->animTime % 9 < 6) {
            enemy->srcRect.x = 112;
        } else {
            enemy->srcRect.x = 128;
        }
    } else {
        enemy->srcRect.x = (enemy->animTime / 3) << 4;
    }

    SDL_RenderCopy(renderer, spriteEnemy, &enemy->srcRect, &enemy->rect);
}

void EnemyDrawStandard(Enemy *enemy) {
    EnemyDrawUFO(enemy, 0);
}

void EnemyDrawShrapnel(Enemy *enemy) {
    EnemyDrawUFO(enemy, 16);
}

void EnemyDraw(Enemy *enemy) {
    if(!enemy->active) {
        return;
    }

    if(enemy->killTime) {
        enemy->srcRect.y = 0;

        if(enemy->killTime < 5) {
            enemy->srcRect.x = 176;
        } else if(enemy->killTime < 8) {
            enemy->srcRect.x = 192;
        } else if(enemy->killTime < 11) {
            enemy->srcRect.x = 208;
        } else {
            enemy->srcRect.x = 224;
        }

        SDL_RenderCopy(renderer, spriteEnemy, &enemy->srcRect, &enemy->rect);
        return;
    }

    if(enemy->type == STANDARD_ENEMY_TYPE) {
        EnemyDrawStandard(enemy);
    } else if(enemy->type == SHRAPNEL_ENEMY_TYPE) {
        EnemyDrawShrapnel(enemy);
    }

    if(DEBUG_HITBOX) {
        if(enemy->hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &enemy->hitbox.rect);
        }
    }
}

void EnemySpawnNext() {
    u8 i = 0;
    while(i < ENEMY_TOTAL) {
        if(!enemies[i].active) {
            enemies[i].rect.x = rand() % 208;
            enemies[i].rect.y = END_Y;
            enemies[i].hitbox.rect.x = enemies[i].rect.x + 4;
            enemies[i].hitbox.rect.y = enemies[i].rect.y + 11;
            enemies[i].hitbox.rect.w = 8;
            enemies[i].hitbox.rect.h = 4;
            enemies[i].hitbox.collidable = 1;
            enemies[i].type = rand() % 10 < 4
                ? SHRAPNEL_ENEMY_TYPE
                : STANDARD_ENEMY_TYPE;

            // Scoring
            switch(enemies[i].type) {
                case SHRAPNEL_ENEMY_TYPE:
                    enemies[i].score = 300;
                    break;
                default:
                    enemies[i].score = 100;
            }

            enemies[i].active = 1;
            return;
        }
        i++;
    }
}
