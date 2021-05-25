#include "defines.h"

SDL_Rect srcRect, destRect, renderClipRect;
SDL_Window *window;
SDL_Renderer *renderer;

void ClearScreen() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
    SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &renderClipRect);
}

int main(int argc, char *argv[]) {
    u16 i = 0;

    // Begin
    SDL_Init(SDL_INIT_VIDEO);

    // Create the video
	window = SDL_CreateWindow("HyperFire Dual Gunners '83",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
							  SCREEN_W, SCREEN_H, WINDOW_FLAGS);
	if(!window) {
		printf("Error creating window.");
		SDL_Quit();
		return 1;
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer) {
        printf("Could not create renderer.");
		SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    renderClipRect.x = (SCREEN_W - PLAYABLE_W) / 2;
    renderClipRect.y = (SCREEN_H - PLAYABLE_H) / 2;
    renderClipRect.w = PLAYABLE_W;
    renderClipRect.h = PLAYABLE_H;
    SDL_RenderSetClipRect(renderer, &renderClipRect);

    const u8 *state = SDL_GetKeyboardState(NULL);

    i = renderClipRect.x;

    while(!state[SDL_SCANCODE_ESCAPE]) {
        SDL_RenderPresent(renderer);

        SDL_PumpEvents(); // Needed for window events. Important!
        state = SDL_GetKeyboardState(NULL);

        ClearScreen();

        destRect.x = i;
        destRect.y = 48;
        destRect.w = 10;
        destRect.h = 10;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &destRect);
        i++;
        if(i >= renderClipRect.x + PLAYABLE_W) {
            i = renderClipRect.x - 10;
        }
    }

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
