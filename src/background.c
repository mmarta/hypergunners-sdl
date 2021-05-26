#include "background.h"

typedef struct {
    u8 r, g, b, on;
    SDL_Rect rect;
} BackgroundStar;

u8 backgroundTime;
BackgroundStar backgroundStars[STAR_TOTAL];

void BackgroundInit() {
    u8 i = 0;
    backgroundTime = 0;

    while(i < STAR_TOTAL) {
        backgroundStars[i].r = rand() % 256;
        backgroundStars[i].g = rand() % 256;
        backgroundStars[i].b = rand() % 256;
        backgroundStars[i].rect.x = (rand() % 224);
        backgroundStars[i].rect.y = (rand() % 64) + 96;
        backgroundStars[i].rect.w = 1;
        backgroundStars[i].rect.h = 1;
        backgroundStars[i].on = i < (STAR_TOTAL >> 1) ? 1 : 0;
        i++;
    }
}

void BackgroundUpdate() {
    u8 i;
    backgroundTime++;
    if(backgroundTime >= 64) {
        backgroundTime = 0;

        i = 0;
        while(i < STAR_TOTAL) {
            if(backgroundStars[i].on) {
                backgroundStars[i].r = rand() % 256;
                backgroundStars[i].g = rand() % 256;
                backgroundStars[i].b = rand() % 256;
                backgroundStars[i].rect.x = (rand() % 224);
                backgroundStars[i].rect.y = (rand() % 64) + 96;
            }
            backgroundStars[i].on = !backgroundStars[i].on;
            i++;
        }
    }
}

void BackgroundDraw() {
    u8 i, lineTime = backgroundTime % 16;
    SDL_SetRenderDrawColor(renderer, BACKGROUND_LINE_R, BACKGROUND_LINE_G, BACKGROUND_LINE_B, 255);

    destRect.x = 0;
    destRect.w = PLAYABLE_W;
    destRect.h = 1;

    destRect.y = 160;
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 160 + lineTime;
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 176 + (lineTime << 1);
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 208 + (lineTime << 2);
    SDL_RenderFillRect(renderer, &destRect);

    destRect.y = 95;
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 95 - lineTime;
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 79 - (lineTime << 1);
    SDL_RenderFillRect(renderer, &destRect);
    destRect.y = 47 - (lineTime << 2);
    SDL_RenderFillRect(renderer, &destRect);

    i = 0;
    while(i < STAR_TOTAL) {
        SDL_SetRenderDrawColor(renderer, backgroundStars[i].r, backgroundStars[i].g, backgroundStars[i].b, 255);
        SDL_RenderFillRect(renderer, &backgroundStars[i].rect);
        i++;
    }
}
