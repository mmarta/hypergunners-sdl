#include "multiple.h"

void MultipleInit(Multiple *multiple, u8 x, u8 y, MultipleType type) {
    u8 i = 0;

    multiple->rect.x = x;
    multiple->rect.y = y;

    multiple->rect.w = MULTIPLE_SIZE;
    multiple->rect.h = MULTIPLE_SIZE;

    multiple->srcRect.y = 16;
    multiple->srcRect.w = MULTIPLE_SIZE;
    multiple->srcRect.h = MULTIPLE_SIZE;

    while(i < MULTIPLE_TRAIL_X_TOTAL) {
        multiple->trailX[i] = x;
        i++;
    }

    multiple->type = type;
    multiple->active = 1;
}

void MultipleFire(Multiple *multiple, u8 playerIndex) {
    if(!multiple->active) {
        return;
    }

    MultipleBulletFireNext(
        playerIndex, multiple->rect.x, multiple->rect.y,
        multiple->type == SPREAD_MULTIPLE ? DOUBLE_BULLET : STANDARD_BULLET
    );
}

void MultipleMoveX(Multiple *multiple, u8 x) {
    u8 i = 0;

    if(!multiple->active) {
        return;
    }

    multiple->rect.x = multiple->trailX[0];
    while(i < MULTIPLE_TRAIL_X_TOTAL - 1) {
        multiple->trailX[i] = multiple->trailX[i + 1];
        i++;
    }

    multiple->trailX[MULTIPLE_TRAIL_X_TOTAL - 1] = x;
}

void MultipleUpdate(Multiple *multiple) {
    if(!multiple->active) {
        return;
    }

    multiple->animTime++;
    if(multiple->animTime >= 16) {
        multiple->animTime = 0;
    }

    switch(multiple->animTime) {
        case 0:
            multiple->srcRect.x = multiple->type == SPREAD_MULTIPLE ? 224 : 192;
            break;
        case 4:
        case 12:
            multiple->srcRect.x = multiple->type == SPREAD_MULTIPLE ? 240 : 208;
            break;
        case 8:
            multiple->srcRect.x = 176;
            break;
    }
}

void MultipleDraw(Multiple *multiple) {
    if(!multiple->active) {
        return;
    }

    SDL_RenderCopy(renderer, spriteEnemy, &multiple->srcRect, &multiple->rect);
}

void MultipleDeactivate(Multiple *multiple) {
    multiple->active = 0;
}
