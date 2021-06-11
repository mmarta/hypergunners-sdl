#include "player.h"

Player players[PLAYER_TOTAL];

void PlayerInit(u8 i) {
    players[i].active = 1;
    players[i].score = 0;
    players[i].lives = 3;
    players[i].animTime = 0;
    players[i].killTime = 0;

    players[i].shootPressed = 0;
    players[i].controllable = 1;
    players[i].hitbox.rect.w = 6;
    players[i].hitbox.rect.h = 6;
    players[i].hitbox.collidable = 1;

    PlayerMove(i, 104, 224);

    players[i].spriteRect.y = 0;
    players[i].spriteRect.w = PLAYER_SIZE;
    players[i].spriteRect.h = PLAYER_SIZE;
    players[i].rect.w = PLAYER_SIZE;
    players[i].rect.h = PLAYER_SIZE;

    players[i].index = i;
}

void PlayerInput(u8 i) {
    if(
        !players[i].active || !players[i].controllable
        || players[i].clawLine.active
    ) {
        return;
    }

    int shootCode, clawCode, leftCode, rightCode;

    // Map scancodes
    if(players[i].index) {
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

    if(controlKeys[leftCode] && players[i].rect.x > 3) {
        PlayerMove(i, players[i].rect.x - 3, players[i].rect.y);
    } else if(controlKeys[rightCode] && players[i].rect.x < 205) {
        PlayerMove(i, players[i].rect.x + 3, players[i].rect.y);
    }

    if(controlKeys[shootCode]) {
        if(!players[i].shootPressed) {
            PlayerBulletFireNext(players[i].index, players[i].rect.x, players[i].rect.y);
            MultipleFire(&players[i].multiple, players[i].index);
            players[i].shootPressed = 1;
        }
    } else if(players[i].shootPressed) {
        players[i].shootPressed = 0;
    }

    if(controlKeys[clawCode]) {
        if(!players[i].clawPressed && !players[i].multiple.active) {
            ClawLineFire(&players[i].clawLine, players[i].rect.x, players[i].rect.y);
            players[i].clawPressed = 1;
        }
    } else if(players[i].clawPressed) {
        players[i].clawPressed = 0;
    }
}

void PlayerMove(u8 i, int x, int y) {
    players[i].rect.x = x;
    players[i].rect.y = y;
    players[i].hitbox.rect.x = x + 5;
    players[i].hitbox.rect.y = y;

    MultipleMoveX(&players[i].multiple, x);
}

void PlayerKill(u8 i) {
    players[i].hitbox.collidable = 0;
    players[i].controllable = 0;
    players[i].killTime = 1;

    MultipleDeactivate(&players[i].multiple);
}

void PlayerUpdate(u8 i) {
    if(!players[i].active) {
        return;
    }

    ClawLineUpdate(&players[i].clawLine);
    MultipleUpdate(&players[i].multiple);

    if(players[i].killTime) {
        if(players[i].killTime == 120) {
            players[i].lives--;
            PlayerMove(i, 104, players[i].rect.y);
            players[i].controllable = 1;
        } else if(players[i].killTime == 240) {
            players[i].killTime = 0;
            players[i].hitbox.collidable = 1;
            players[i].animTime = 0;
            return;
        }

        players[i].killTime++;
    }

    players[i].animTime++;
    if(players[i].animTime >= 12) {
        players[i].animTime = 0;
    }
}

void PlayerDraw(u8 i) {
    u8 killMod, noDraw = 0;
    if(!players[i].active) {
        return;
    }

    if(players[i].index) {
        PrintFont(184, 0, "2P");
        PrintFontNumber(216, 8, players[i].score, 99999999);
    } else {
        PrintFont(24, 0, "1P");
        PrintFontNumber(56, 8, players[i].score, 99999999);
    }

    // Draw ClawLine & Multiple
    ClawLineDraw(&players[i].clawLine);
    MultipleDraw(&players[i].multiple);

    // Dying?
    if(players[i].killTime) {
        if(players[i].killTime < 5) {
            players[i].spriteRect.x = 176;
        } else if(players[i].killTime < 9) {
            players[i].spriteRect.x = 192;
        } else if(players[i].killTime < 13) {
            players[i].spriteRect.x = 208;
        } else if(players[i].killTime < 17) {
            players[i].spriteRect.x = 224;
        } else if(players[i].killTime <= 120) {
            noDraw = 1;
        } else if(players[i].killTime > 120) {
            killMod = players[i].killTime % 8;
            if(killMod >= 4) {
                noDraw = 1;
            } else {
                players[i].spriteRect.x = players[i].index ? 64 : 0;
            }
        }

        if(noDraw) {
            return;
        }

        SDL_RenderCopy(renderer, spritePlayer, &players[i].spriteRect, &players[i].rect);

        return;
    }

    if(players[i].index) {
        if(players[i].animTime < 3) {
            players[i].spriteRect.x = 80;
        } else if(players[i].animTime < 6 || players[i].animTime >= 9) {
            players[i].spriteRect.x = 96;
        } else {
            players[i].spriteRect.x = 112;
        }
    } else {
        if(players[i].animTime < 3) {
            players[i].spriteRect.x = 16;
        } else if(players[i].animTime < 6 || players[i].animTime >= 9) {
            players[i].spriteRect.x = 32;
        } else {
            players[i].spriteRect.x = 48;
        }
    }

    SDL_RenderCopy(renderer, spritePlayer, &players[i].spriteRect, &players[i].rect);

    if(DEBUG_HITBOX) {
        if(players[i].hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &players[i].hitbox.rect);
        }
    }
}
