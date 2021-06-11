#include "bullet.h"

/***
* PLAYER BULLET CODE
*/

PlayerBullet playerBullets[PLAYER_BULLET_TOTAL];
ShrapnelBullet shrapnelBullets[SHRAPNEL_BULLET_TOTAL];
PlayerBullet multipleBullets[MULTIPLE_BULLET_TOTAL];

void PlayerBulletActivate(PlayerBullet *, u8, u8, PlayerBulletType);

void PlayerBulletFireNext(u8 playerIndex, int x, int y) {
    u8 i = playerIndex * PLAYER_BULLET_PER_PLAYER;
    u8 end = i + PLAYER_BULLET_PER_PLAYER;

    while(i < end) {
        if(!playerBullets[i].active) {
            PlayerBulletActivate(&playerBullets[i], x, y, STANDARD_BULLET);
            return;
        }
        i++;
    }
}

void PlayerBulletActivate(PlayerBullet *playerBullet, u8 x, u8 y, PlayerBulletType type) {
    playerBullet->rect.x = x;
    playerBullet->rect.y = y;
    playerBullet->rect.w = PLAYER_BULLET_W;
    playerBullet->rect.h = PLAYER_BULLET_H;
    playerBullet->hitbox.rect.x = playerBullet->rect.x;
    playerBullet->hitbox.rect.y = playerBullet->rect.y;
    playerBullet->hitbox.rect.w = playerBullet->rect.w;
    playerBullet->hitbox.rect.h = playerBullet->rect.h;
    playerBullet->srcRect.x = 0;
    playerBullet->srcRect.y = 0;
    playerBullet->srcRect.w = playerBullet->rect.w;
    playerBullet->srcRect.h = playerBullet->rect.h;
    playerBullet->hitbox.collidable = 1;
    playerBullet->active = 1;
}

void PlayerBulletUpdate(PlayerBullet *playerBullet) {
    if(!playerBullet->active) {
        return;
    }

    if(playerBullet->rect.y < FAR_Y) {
        playerBullet->rect.y -= 2;
        playerBullet->hitbox.rect.y -= 2;
        if(playerBullet->rect.y < END_Y) {
            PlayerBulletDeactivate(playerBullet);
        }
    } else if(playerBullet->rect.y < MIDDLE_Y) {
        playerBullet->rect.y -= 3;
        playerBullet->hitbox.rect.y -= 3;
        if(playerBullet->rect.y < FAR_Y) {
            playerBullet->hitbox.rect.x += 2;
            playerBullet->hitbox.rect.w -= 4;
            playerBullet->hitbox.rect.h -= 2;
        }
    } else {
        playerBullet->rect.y -= 4;
        playerBullet->hitbox.rect.y -= 4;
        if(playerBullet->rect.y < MIDDLE_Y) {
            playerBullet->hitbox.rect.x += 2;
            playerBullet->hitbox.rect.w -= 4;
            playerBullet->hitbox.rect.h -= 4;
        }
    }
}

void PlayerBulletDraw(PlayerBullet *playerBullet) {
    if(!playerBullet->active) {
        return;
    }

    if(playerBullet->rect.y < FAR_Y) {
        playerBullet->srcRect.x = 160;
    } else if(playerBullet->rect.y < MIDDLE_Y) {
        playerBullet->srcRect.x = 144;
    } else {
        playerBullet->srcRect.x = 128;
    }

    SDL_RenderCopy(renderer, spritePlayer, &playerBullet->srcRect, &playerBullet->rect);

    if(DEBUG_HITBOX) {
        if(playerBullet->hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &playerBullet->hitbox.rect);
        }
    }
}

void PlayerBulletDeactivate(PlayerBullet *playerBullet) {
    playerBullet->hitbox.collidable = 0;
    playerBullet->active = 0;
}

/***
* MULTIPLE BULLET CODE (EXTENSION OF PLAYER)
*/

void MultipleBulletFireNext(u8 playerIndex, int x, int y, PlayerBulletType type) {
    u8 i = playerIndex * MULTIPLE_BULLET_PER_PLAYER;
    u8 end = i + PLAYER_BULLET_PER_PLAYER;
    u8 greaterEnd;

    switch(type) {
        case DOUBLE_BULLET:
            greaterEnd = i + MULTIPLE_BULLET_PER_PLAYER;
            x -= (PLAYER_BULLET_W >> 1);
            break;
        default:
            greaterEnd = i + PLAYER_BULLET_PER_PLAYER;
    }

    while(i < greaterEnd) {
        while(i < end) {
            if(!multipleBullets[i].active) {
                PlayerBulletActivate(&multipleBullets[i], x, y, type);
                break;
            }
            i++;
        }

        i = end;
        x += PLAYER_BULLET_W;
        end += PLAYER_BULLET_PER_PLAYER;
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

void ShrapnelBulletUpdate(u8 i) {
    if(!shrapnelBullets[i].active) {
        return;
    }

    if(
        shrapnelBullets[i].rect.x + shrapnelBullets[i].rect.w <= 0
        || shrapnelBullets[i].rect.x >= PLAYABLE_W
    ) {
        ShrapnelBulletDeactivate(i);
    } else if(
        shrapnelBullets[i].rect.y + shrapnelBullets[i].rect.h <= 0
        || shrapnelBullets[i].rect.y >= PLAYABLE_H
    ) {
        ShrapnelBulletDeactivate(i);
    }

    shrapnelBullets[i].rect.x += shrapnelBullets[i].dX;
    shrapnelBullets[i].rect.y += shrapnelBullets[i].dY;
    shrapnelBullets[i].hitbox.rect.x += shrapnelBullets[i].dX;
    shrapnelBullets[i].hitbox.rect.y += shrapnelBullets[i].dY;
}

void ShrapnelBulletDraw(u8 i) {
    if(!shrapnelBullets[i].active) {
        return;
    }

    SDL_RenderCopy(renderer, spriteEnemy, &shrapnelBullets[i].srcRect, &shrapnelBullets[i].rect);
}

void ShrapnelBulletDeactivate(u8 i) {
    shrapnelBullets[i].hitbox.collidable = 0;
    shrapnelBullets[i].active = 0;
}
