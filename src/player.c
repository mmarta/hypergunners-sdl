#include "player.h"

Player players[PLAYER_TOTAL];

void PlayerInit(Player *player, u8 i) {
    player->active = 1;
    player->score = 0;
    player->lives = 3;
    player->animTime = 0;
    player->killTime = 0;

    player->shootPressed = 0;
    player->controllable = 1;
    player->hitbox.rect.w = 6;
    player->hitbox.rect.h = 6;
    player->hitbox.collidable = 1;

    PlayerMove(player, 104, 224);

    player->spriteRect.y = 0;
    player->spriteRect.w = PLAYER_SIZE;
    player->spriteRect.h = PLAYER_SIZE;
    player->rect.w = PLAYER_SIZE;
    player->rect.h = PLAYER_SIZE;

    player->index = i;
}

void PlayerInput(Player *player) {
    if(
        !player->active || !player->controllable
        || player->clawLine.active
    ) {
        return;
    }

    int shootCode, clawCode, leftCode, rightCode;

    // Map scancodes
    if(player->index) {
        shootCode = SDL_SCANCODE_G;
        clawCode = SDL_SCANCODE_H;
        leftCode = SDL_SCANCODE_A;
        rightCode = SDL_SCANCODE_D;
    } else {
        shootCode = SDL_SCANCODE_Z;
        clawCode = SDL_SCANCODE_X;
        leftCode = SDL_SCANCODE_LEFT;
        rightCode = SDL_SCANCODE_RIGHT;
    }

    if(controlKeys[leftCode] && player->rect.x > 3) {
        PlayerMove(player, player->rect.x - 3, player->rect.y);
    } else if(controlKeys[rightCode] && player->rect.x < 205) {
        PlayerMove(player, player->rect.x + 3, player->rect.y);
    }

    if(controlKeys[shootCode]) {
        if(!player->shootPressed) {
            PlayerBulletFireNext(player->bullets, player->rect.x, player->rect.y, PLAYER_BULLETS_COUNT);
            MultipleFire(&player->multiple);
            player->shootPressed = 1;
        }
    } else if(player->shootPressed) {
        player->shootPressed = 0;
    }

    if(controlKeys[clawCode]) {
        if(!player->clawPressed && !player->multiple.active) {
            ClawLineFire(&player->clawLine, player->rect.x, player->rect.y);
            player->clawPressed = 1;
        }
    } else if(player->clawPressed) {
        player->clawPressed = 0;
    }
}

void PlayerMove(Player *player, int x, int y) {
    player->rect.x = x;
    player->rect.y = y;
    player->hitbox.rect.x = x + 5;
    player->hitbox.rect.y = y;

    MultipleMoveX(&player->multiple, x);
}

void PlayerKill(Player *player) {
    player->hitbox.collidable = 0;
    player->controllable = 0;
    player->killTime = 1;

    MultipleDeactivate(&player->multiple);
}

void PlayerUpdate(Player *player) {
    u8 i;
    if(!player->active) {
        return;
    }

    ClawLineUpdate(&player->clawLine);
    MultipleUpdate(&player->multiple);

    i = 0;
    while(i < PLAYER_BULLETS_COUNT) {
        PlayerBulletUpdate(&player->bullets[i]);
        i++;
    }

    if(player->killTime) {
        if(player->killTime == 120) {
            player->lives--;
            PlayerMove(player, 104, player->rect.y);
            player->controllable = 1;
        } else if(player->killTime == 240) {
            player->killTime = 0;
            player->hitbox.collidable = 1;
            player->animTime = 0;
            return;
        }

        player->killTime++;
    }

    player->animTime++;
    if(player->animTime >= 12) {
        player->animTime = 0;
    }
}

void PlayerDraw(Player *player) {
    u8 i, killMod, noDraw = 0;
    if(!player->active) {
        return;
    }

    if(player->index) {
        PrintFont(184, 0, "2P");
        PrintFontNumber(216, 8, player->score, 99999999);
    } else {
        PrintFont(24, 0, "1P");
        PrintFontNumber(56, 8, player->score, 99999999);
    }

    // Draw ClawLine & Multiple
    ClawLineDraw(&player->clawLine);
    MultipleDraw(&player->multiple);

    // Draw player's bullets
    i = 0;
    while(i < PLAYER_BULLETS_COUNT) {
        PlayerBulletDraw(&player->bullets[i]);
        i++;
    }

    // Dying?
    if(player->killTime) {
        if(player->killTime < 5) {
            player->spriteRect.x = 176;
        } else if(player->killTime < 9) {
            player->spriteRect.x = 192;
        } else if(player->killTime < 13) {
            player->spriteRect.x = 208;
        } else if(player->killTime < 17) {
            player->spriteRect.x = 224;
        } else if(player->killTime <= 120) {
            noDraw = 1;
        } else if(player->killTime > 120) {
            killMod = player->killTime % 8;
            if(killMod >= 4) {
                noDraw = 1;
            } else {
                player->spriteRect.x = player->index ? 64 : 0;
            }
        }

        if(noDraw) {
            return;
        }

        SDL_RenderCopy(renderer, spritePlayer, &player->spriteRect, &player->rect);

        return;
    }

    if(player->index) {
        if(player->animTime < 3) {
            player->spriteRect.x = 80;
        } else if(player->animTime < 6 || player->animTime >= 9) {
            player->spriteRect.x = 96;
        } else {
            player->spriteRect.x = 112;
        }
    } else {
        if(player->animTime < 3) {
            player->spriteRect.x = 16;
        } else if(player->animTime < 6 || player->animTime >= 9) {
            player->spriteRect.x = 32;
        } else {
            player->spriteRect.x = 48;
        }
    }

    SDL_RenderCopy(renderer, spritePlayer, &player->spriteRect, &player->rect);

    if(DEBUG_HITBOX) {
        if(player->hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &player->hitbox.rect);
        }
    }
}
