#include "bullet.h"

/***
* PLAYER BULLET CODE
*/

EnemyBullet shrapnelBullets[SHRAPNEL_BULLET_TOTAL];

void PlayerBulletActivate(PlayerBullet *, u8, u8, PlayerBulletType);

void PlayerBulletFireNext(PlayerBullet bullets[], int x, int y, u8 count) {
    u8 i = 0;

    while(i < count) {
        if(!bullets[i].active) {
            PlayerBulletActivate(&bullets[i], x, y, STANDARD_PLAYER_BULLET_TYPE);
            return;
        }
        i++;
    }
}

void PlayerBulletActivate(PlayerBullet *bullet, u8 x, u8 y, PlayerBulletType type) {
    bullet->rect.x = x;
    bullet->rect.y = y;
    bullet->rect.w = PLAYER_BULLET_W;
    bullet->rect.h = PLAYER_BULLET_H;
    bullet->hitbox.rect.x = bullet->rect.x;
    bullet->hitbox.rect.y = bullet->rect.y;
    bullet->hitbox.rect.w = bullet->rect.w;
    bullet->hitbox.rect.h = bullet->rect.h;
    bullet->srcRect.x = 0;
    bullet->srcRect.y = 0;
    bullet->srcRect.w = bullet->rect.w;
    bullet->srcRect.h = bullet->rect.h;
    bullet->hitbox.collidable = 1;
    bullet->active = 1;
}

void PlayerBulletUpdate(PlayerBullet *bullet) {
    if(!bullet->active) {
        return;
    }

    if(bullet->rect.y < FAR_Y) {
        bullet->rect.y -= 2;
        bullet->hitbox.rect.y -= 2;
        if(bullet->rect.y < END_Y) {
            PlayerBulletDeactivate(bullet);
        }
    } else if(bullet->rect.y < MIDDLE_Y) {
        bullet->rect.y -= 3;
        bullet->hitbox.rect.y -= 3;
        if(bullet->rect.y < FAR_Y) {
            bullet->hitbox.rect.x += 2;
            bullet->hitbox.rect.w -= 4;
            bullet->hitbox.rect.h -= 2;
        }
    } else {
        bullet->rect.y -= 4;
        bullet->hitbox.rect.y -= 4;
        if(bullet->rect.y < MIDDLE_Y) {
            bullet->hitbox.rect.x += 2;
            bullet->hitbox.rect.w -= 4;
            bullet->hitbox.rect.h -= 4;
        }
    }
}

void PlayerBulletDraw(PlayerBullet *bullet) {
    if(!bullet->active) {
        return;
    }

    if(bullet->rect.y < FAR_Y) {
        bullet->srcRect.x = 160;
    } else if(bullet->rect.y < MIDDLE_Y) {
        bullet->srcRect.x = 144;
    } else {
        bullet->srcRect.x = 128;
    }

    SDL_RenderCopy(renderer, spritePlayer, &bullet->srcRect, &bullet->rect);

    if(DEBUG_HITBOX) {
        if(bullet->hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &bullet->hitbox.rect);
        }
    }
}

void PlayerBulletDeactivate(PlayerBullet *bullet) {
    bullet->hitbox.collidable = 0;
    bullet->active = 0;
}

/***
* MULTIPLE BULLET CODE (EXTENSION OF PLAYER)
*/

void MultipleBulletFireNext(PlayerBullet bullets[], int x, int y, PlayerBulletType type, u8 roundCount, u8 fullCount) {
    u8 i = 0;
    u8 greaterEnd;

    switch(type) {
        case DOUBLE_PLAYER_BULLET_TYPE:
            greaterEnd = fullCount;
            x -= (PLAYER_BULLET_W >> 1);
            break;
        default:
            greaterEnd = roundCount;
    }

    while(i < greaterEnd) {
        while(i < roundCount) {
            if(!bullets[i].active) {
                PlayerBulletActivate(&bullets[i], x, y, type);
                break;
            }
            i++;
        }

        i = roundCount;
        x += PLAYER_BULLET_W;
        roundCount += roundCount;
    }

}

/***
* SHRAPNEL CODE
*/

void ShrapnelBulletSpray(int enemyX, int enemyY, u8 associatedPlayerIndex, u8 multiplier) {
    u8 i = 0, sprayed = 0;
    while(i < SHRAPNEL_BULLET_TOTAL) {
        if(!shrapnelBullets[i].active) {
            shrapnelBullets[i].rect.x = sprayed < 2 ? enemyX : enemyX + SHRAPNEL_BULLET_SIZE;
            shrapnelBullets[i].rect.y = sprayed % 2 ? enemyY + SHRAPNEL_BULLET_SIZE : enemyY;
            shrapnelBullets[i].dX = sprayed < 2 ? -4 : 4;
            shrapnelBullets[i].dY = sprayed % 2 ? 2 : -4;
            shrapnelBullets[i].associatedPlayerIndex = associatedPlayerIndex;
            shrapnelBullets[i].multiplier = multiplier;
            shrapnelBullets[i].hitbox.rect.x = shrapnelBullets[i].rect.x;
            shrapnelBullets[i].hitbox.rect.y = shrapnelBullets[i].rect.y;
            shrapnelBullets[i].rect.w = SHRAPNEL_BULLET_SIZE;
            shrapnelBullets[i].rect.h = SHRAPNEL_BULLET_SIZE;
            shrapnelBullets[i].hitbox.rect.w = SHRAPNEL_BULLET_SIZE;
            shrapnelBullets[i].hitbox.rect.h = SHRAPNEL_BULLET_SIZE;
            shrapnelBullets[i].srcRect.x = shrapnelBullets[i].dX < 0 ? 240 : 248;
            shrapnelBullets[i].srcRect.y = shrapnelBullets[i].dY < 0 ? 0 : 8;
            shrapnelBullets[i].srcRect.w = shrapnelBullets[i].rect.w;
            shrapnelBullets[i].srcRect.h = shrapnelBullets[i].rect.h;
            shrapnelBullets[i].hitbox.collidable = 1;
            shrapnelBullets[i].active = 1;

            sprayed++;
            if(sprayed >= 4) {
                return;
            }
        }
        i++;
    }
}

void ShrapnelBulletUpdate(EnemyBullet *bullet) {
    if(!bullet->active) {
        return;
    }

    if(
        bullet->rect.x + bullet->rect.w <= 0
        || bullet->rect.x >= PLAYABLE_W
    ) {
        ShrapnelBulletDeactivate(bullet);
    } else if(
        bullet->rect.y + bullet->rect.h <= 0
        || bullet->rect.y >= PLAYABLE_H
    ) {
        ShrapnelBulletDeactivate(bullet);
    }

    bullet->rect.x += bullet->dX;
    bullet->rect.y += bullet->dY;
    bullet->hitbox.rect.x += bullet->dX;
    bullet->hitbox.rect.y += bullet->dY;
}

void ShrapnelBulletDraw(EnemyBullet *bullet) {
    if(!bullet->active) {
        return;
    }

    SDL_RenderCopy(renderer, spriteEnemy, &bullet->srcRect, &bullet->rect);
}

void ShrapnelBulletDeactivate(EnemyBullet *bullet) {
    bullet->hitbox.collidable = 0;
    bullet->active = 0;
}
