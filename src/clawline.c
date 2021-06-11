#include "clawline.h"

void ClawLineMove(ClawLine *, u8, u8);

void ClawLineFire(ClawLine *clawLine, u8 x, u8 y) {
    if(clawLine->active) {
        return;
    }

    clawLine->rect.w = CLAWLINE_SIZE;
    clawLine->rect.h = CLAWLINE_SIZE;
    clawLine->srcRect.w = CLAWLINE_SIZE;
    clawLine->srcRect.h = CLAWLINE_SIZE;

    ClawLineMove(clawLine, x, y);

    clawLine->grabbed = 0;
    clawLine->hitbox.rect.w = 14;
    clawLine->hitbox.rect.h = 13;
    clawLine->hitbox.collidable = 1;
    clawLine->returning = 0;
    clawLine->active = 1;
}

void ClawLineMove(ClawLine *clawLine, u8 x, u8 y) {
    clawLine->rect.x = x;
    clawLine->rect.y = y;

    if(y < FAR_Y) {
        clawLine->srcRect.x = clawLine->grabbed ? 64 : 16;
        clawLine->srcRect.y = 16;

        clawLine->hitbox.rect.x = x + 4;
        clawLine->hitbox.rect.y = y + 8;
    } else if(y < MIDDLE_Y) {
        clawLine->srcRect.x = clawLine->grabbed ? 48 : 0;
        clawLine->srcRect.y = 16;

        clawLine->hitbox.rect.x = x + 2;
        clawLine->hitbox.rect.y = y + 4;
    } else {
        clawLine->srcRect.x = clawLine->grabbed ? 32 : 240;
        clawLine->srcRect.y = clawLine->grabbed ? 16 : 0;

        clawLine->hitbox.rect.x = x + 1;
        clawLine->hitbox.rect.y = y;
    }
}

void ClawLineReturn(ClawLine *clawLine, u8 grab) {
    clawLine->returning = 1;
    clawLine->grabbed = grab;
    clawLine->hitbox.collidable = 0;
}

void ClawLineUpdate(ClawLine *clawLine) {
    if(!clawLine->active) {
        return;
    }

    if(clawLine->returning) {
        if(clawLine->rect.y < FAR_Y) {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y + 8);
            if(clawLine->rect.y >= FAR_Y) {
                clawLine->hitbox.rect.w = 12;
                clawLine->hitbox.rect.h = 9;
            }
        } else if(clawLine->rect.y < MIDDLE_Y) {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y + 12);
            if(clawLine->rect.y >= MIDDLE_Y) {
                clawLine->hitbox.rect.w = 14;
                clawLine->hitbox.rect.h = 13;
            }
        } else {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y + 16);
            if(clawLine->rect.y >= CLAWLINE_START_Y) {
                ClawLineDeactivate(clawLine);
            }
        }
    } else {
        if(clawLine->rect.y < END_Y) {
            ClawLineReturn(clawLine, 0);
        } else if(clawLine->rect.y < FAR_Y) {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y - 4);
        } else if(clawLine->rect.y < MIDDLE_Y) {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y - 6);
            if(clawLine->rect.y < FAR_Y) {
                clawLine->hitbox.rect.w = 8;
                clawLine->hitbox.rect.h = 6;
            }
        } else {
            ClawLineMove(clawLine, clawLine->rect.x, clawLine->rect.y - 8);
            if(clawLine->rect.y < MIDDLE_Y) {
                clawLine->hitbox.rect.w = 12;
                clawLine->hitbox.rect.h = 9;
            }
        }
    }
}

void ClawLineDraw(ClawLine *clawLine) {
    if(!clawLine->active) {
        return;
    }

    SDL_RenderCopy(renderer, spritePlayer, &clawLine->srcRect, &clawLine->rect);

    if(DEBUG_HITBOX) {
        if(clawLine->hitbox.collidable) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);
            SDL_RenderFillRect(renderer, &clawLine->hitbox.rect);
        }
    }

    srcRect.x = 80;
    srcRect.y = 16;
    srcRect.w = CLAWLINE_SIZE;
    srcRect.h = CLAWLINE_SIZE;

    destRect.x = clawLine->rect.x;
    destRect.y = clawLine->rect.y + CLAWLINE_SIZE;
    destRect.w = CLAWLINE_SIZE;
    destRect.h = CLAWLINE_SIZE;

    while(destRect.y < CLAWLINE_START_Y) {
        SDL_RenderCopy(renderer, spritePlayer, &srcRect, &destRect);
        destRect.y += CLAWLINE_SIZE;
    }
}

void ClawLineDeactivate(ClawLine *clawLine) {
    clawLine->returning = 0;
    clawLine->grabbed = 0;
    clawLine->hitbox.collidable = 0;
    clawLine->active = 0;
}
