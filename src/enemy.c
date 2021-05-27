#include "enemy.h"

void EnemyInit(u8);
void EnemyDrawUFO(u8, u8);
void EnemyDrawStandard(u8);
void EnemyDrawShrapnel(u8);

Enemy enemies[ENEMY_TOTAL];

void EnemyInitAll() {
    u8 i = 0;
    while(i < ENEMY_TOTAL) {
        EnemyInit(i);
        i++;
    }
}

void EnemyInit(u8 i) {
    enemies[i].rect.w = ENEMY_SIZE;
    enemies[i].rect.h = ENEMY_SIZE;
    enemies[i].srcRect.w = ENEMY_SIZE;
    enemies[i].srcRect.h = ENEMY_SIZE;
    enemies[i].active = 0;
}

void EnemyKill(u8 i, Player *player, u8 multiplier) {
    if(!multiplier) {
        multiplier = 1;
    }

    enemies[i].killTime = 1;
    enemies[i].multiplier = multiplier;
    enemies[i].hitbox.collidable = 0;
    if(enemies[i].type == ENEMY_TYPE_SHRAPNEL) {
        ShrapnelBulletSpray(enemies[i].rect.x, enemies[i].rect.y, player->index, multiplier == 32 ? multiplier : multiplier * 2);
    }
    player->score += (enemies[i].score * enemies[i].multiplier);
}

void EnemyDeactivate(u8 i) {
    enemies[i].killTime = 0;
    enemies[i].hitbox.collidable = 0;
    enemies[i].active = 0;
}

void EnemyUpdate(u8 i) {
    if(!enemies[i].active) {
        return;
    }

    if(enemies[i].killTime) {
        enemies[i].killTime++;
        if(enemies[i].killTime >= 14) {
            EnemyDeactivate(i);
        }
        return;
    }

    enemies[i].animTime++;
    if(enemies[i].animTime >= 18) {
        enemies[i].animTime = 0;
    }

    if(enemies[i].rect.y < FAR_Y) {
        if(!(enemies[i].animTime % 2)) {
            enemies[i].rect.y++;
            enemies[i].hitbox.rect.y++;
        }
        if(enemies[i].rect.y >= FAR_Y) {
            enemies[i].hitbox.rect.x -= 2;
            enemies[i].hitbox.rect.w += 4;
            enemies[i].hitbox.rect.y -= 2;
            enemies[i].hitbox.rect.h++;
        }
    } else if(enemies[i].rect.y < MIDDLE_Y) {
        enemies[i].rect.y++;
        enemies[i].hitbox.rect.y++;
        if(enemies[i].rect.y >= MIDDLE_Y) {
            enemies[i].hitbox.rect.x -= 2;
            enemies[i].hitbox.rect.w += 4;
            enemies[i].hitbox.rect.y -= 3;
            enemies[i].hitbox.rect.h++;
        }
    } else {
        enemies[i].rect.y++;
        enemies[i].hitbox.rect.y++;
        if(enemies[i].rect.y >= 240) {
            enemies[i].hitbox.collidable = 0;
            enemies[i].active = 0;
        }
    }
}

void EnemyDrawUFO(u8 i, u8 srcY) {
    enemies[i].srcRect.y = srcY;

    if(enemies[i].rect.y < FAR_Y) {
        if(enemies[i].animTime % 6 < 3) {
            enemies[i].srcRect.x = 144;
        } else {
            enemies[i].srcRect.x = 160;
        }
    } else if(enemies[i].rect.y < MIDDLE_Y) {
        if(enemies[i].animTime % 9 < 3) {
            enemies[i].srcRect.x = 96;
        } else if(enemies[i].animTime % 9 < 6) {
            enemies[i].srcRect.x = 112;
        } else {
            enemies[i].srcRect.x = 128;
        }
    } else {
        enemies[i].srcRect.x = (enemies[i].animTime / 3) << 4;
    }

    SDL_RenderCopy(renderer, spriteEnemy, &enemies[i].srcRect, &enemies[i].rect);
}

void EnemyDrawStandard(u8 i) {
    EnemyDrawUFO(i, 0);
}

void EnemyDrawShrapnel(u8 i) {
    EnemyDrawUFO(i, 16);
}

void EnemyDraw(u8 i) {
    if(!enemies[i].active) {
        return;
    }

    if(enemies[i].killTime) {
        enemies[i].srcRect.y = 0;

        if(enemies[i].killTime < 5) {
            enemies[i].srcRect.x = 176;
        } else if(enemies[i].killTime < 8) {
            enemies[i].srcRect.x = 192;
        } else if(enemies[i].killTime < 11) {
            enemies[i].srcRect.x = 208;
        } else {
            enemies[i].srcRect.x = 224;
        }

        SDL_RenderCopy(renderer, spriteEnemy, &enemies[i].srcRect, &enemies[i].rect);
        return;
    }

    if(enemies[i].type == ENEMY_TYPE_STANDARD) {
        EnemyDrawStandard(i);
    } else if(enemies[i].type == ENEMY_TYPE_SHRAPNEL) {
        EnemyDrawShrapnel(i);
    }

    if(DEBUG_HITBOX) {
        if(enemies[i].hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &enemies[i].hitbox.rect);
        }
    }
}

void EnemySpawnNext() {
    u8 i = 0;
    while(i < ENEMY_TOTAL) {
        if(!enemies[i].active) {
            enemies[i].rect.x = rand() % 208;
            enemies[i].rect.y = 128;
            enemies[i].hitbox.rect.x = enemies[i].rect.x + 4;
            enemies[i].hitbox.rect.y = enemies[i].rect.y + 11;
            enemies[i].hitbox.rect.w = 8;
            enemies[i].hitbox.rect.h = 4;
            enemies[i].hitbox.collidable = 1;
            enemies[i].type = rand() % 10 < 4
                ? ENEMY_TYPE_SHRAPNEL
                : ENEMY_TYPE_STANDARD;

            // Scoring
            switch(enemies[i].type) {
                case ENEMY_TYPE_SHRAPNEL:
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
