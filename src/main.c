#include "defines.h"

SDL_Rect srcRect, destRect, renderClipRect;
SDL_Surface *screen;

void ClearScreen() {
    SDL_FillRect(screen, &renderClipRect, 0);
}

int main(int argc, char *argv[]) {
    u16 i = 0;

    // Begin
    SDL_Init(SDL_INIT_VIDEO);

    // Create the video
    screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, 32, VIDEO_FLAGS);

    if(!screen) {
        printf("Could not create video: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderClipRect.x = 0;
    renderClipRect.y = 0;
    renderClipRect.w = SCREEN_W;
    renderClipRect.h = SCREEN_H;
    SDL_FillRect(screen, &renderClipRect, 0x202020ff);

    renderClipRect.x = (SCREEN_W - PLAYABLE_W) / 2;
    renderClipRect.y = (SCREEN_H - PLAYABLE_H) / 2;
    renderClipRect.w = PLAYABLE_W;
    renderClipRect.h = PLAYABLE_H;
    SDL_SetClipRect(screen, &renderClipRect);

    const u8 *state = SDL_GetKeyState(NULL);

    i = renderClipRect.x;

    while(!state[SDLK_ESCAPE]) {
        SDL_Flip(screen);

        SDL_PumpEvents(); // Needed for window events. Important!
        state = SDL_GetKeyState(NULL);

        ClearScreen();

        destRect.x = i;
        destRect.y = 48;
        destRect.w = 10;
        destRect.h = 10;
        SDL_FillRect(screen, &destRect, 0x0000ff00);
        i++;
        if(i >= renderClipRect.x + PLAYABLE_W) {
            i = renderClipRect.x - 10;
        }
    }

    SDL_Quit();
    return 0;
}
