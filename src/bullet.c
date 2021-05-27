#include "bullet.h"

/***
* PLAYER BULLET CODE
*/

PlayerBullet playerBullets[PLAYER_BULLET_TOTAL];
ShrapnelBullet shrapnelBullets[SHRAPNEL_BULLET_TOTAL];

void PlayerBulletFireNext(u8 playerIndex, int x, int y) {
    u8 i = playerIndex << 1;
    u8 end = i + PLAYER_BULLET_PER_PLAYER;

    while(i < end) {
        if(!playerBullets[i].active) {
            playerBullets[i].rect.x = x;
            playerBullets[i].rect.y = y;
            playerBullets[i].rect.w = PLAYER_BULLET_W;
            playerBullets[i].rect.h = PLAYER_BULLET_H;
            playerBullets[i].hitbox.rect.x = playerBullets[i].rect.x;
            playerBullets[i].hitbox.rect.y = playerBullets[i].rect.y;
            playerBullets[i].hitbox.rect.w = playerBullets[i].rect.w;
            playerBullets[i].hitbox.rect.h = playerBullets[i].rect.h;
            playerBullets[i].srcRect.x = 0;
            playerBullets[i].srcRect.y = 0;
            playerBullets[i].srcRect.w = playerBullets[i].rect.w;
            playerBullets[i].srcRect.h = playerBullets[i].rect.h;
            playerBullets[i].hitbox.collidable = 1;
            playerBullets[i].active = 1;
            return;
        }
        i++;
    }
}

void PlayerBulletUpdate(u8 i) {
    if(!playerBullets[i].active) {
        return;
    }

    if(playerBullets[i].rect.y < FAR_Y) {
        playerBullets[i].rect.y -= 2;
        playerBullets[i].hitbox.rect.y -= 2;
        if(playerBullets[i].rect.y < END_Y) {
            PlayerBulletDeactivate(i);
        }
    } else if(playerBullets[i].rect.y < MIDDLE_Y) {
        playerBullets[i].rect.y -= 3;
        playerBullets[i].hitbox.rect.y -= 3;
        if(playerBullets[i].rect.y < FAR_Y) {
            playerBullets[i].hitbox.rect.x += 2;
            playerBullets[i].hitbox.rect.w -= 4;
            playerBullets[i].hitbox.rect.h -= 2;
        }
    } else {
        playerBullets[i].rect.y -= 4;
        playerBullets[i].hitbox.rect.y -= 4;
        if(playerBullets[i].rect.y < MIDDLE_Y) {
            playerBullets[i].hitbox.rect.x += 2;
            playerBullets[i].hitbox.rect.w -= 4;
            playerBullets[i].hitbox.rect.h -= 4;
        }
    }
}

void PlayerBulletDraw(u8 i) {
    if(!playerBullets[i].active) {
        return;
    }

    if(playerBullets[i].rect.y < FAR_Y) {
        playerBullets[i].srcRect.x = 160;
    } else if(playerBullets[i].rect.y < MIDDLE_Y) {
        playerBullets[i].srcRect.x = 144;
    } else {
        playerBullets[i].srcRect.x = 128;
    }

    SDL_RenderCopy(renderer, spritePlayer, &playerBullets[i].srcRect, &playerBullets[i].rect);

    if(DEBUG_HITBOX) {
        if(playerBullets[i].hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &playerBullets[i].hitbox.rect);
        }
    }
}

void PlayerBulletDeactivate(u8 i) {
    playerBullets[i].hitbox.collidable = 0;
    playerBullets[i].active = 0;
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
            shrapnelBullets[i].dY = sprayed % 2 ? 4 : -4;
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
