#include "defines.h"
#include "graphics.h"

SDL_Rect renderClipRect;

void ClearScreen() {
	SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
    SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &renderClipRect);
}

int main(int argc, char *argv[]) {
    u16 i = 0;
	u8 animTime = 0;

    // Begin
    SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

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

	if(LoadGraphics() != 0) {
		printf("Could not create graphics.");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
	}

    while(!state[SDL_SCANCODE_ESCAPE]) {
        SDL_RenderPresent(renderer);

        SDL_PumpEvents(); // Needed for window events. Important!
        state = SDL_GetKeyboardState(NULL);

        ClearScreen();

		// Blit Sprite!
		if(animTime < 3) {
			srcRect.x = 16;
		} else if(animTime < 6 || animTime >= 9) {
			srcRect.x = 32;
		} else {
			srcRect.x = 48;
		}
		srcRect.y = 0;
		srcRect.w = 16;
		srcRect.h = 16;

		destRect.x = i;
		destRect.y = 32;
		destRect.w = 16;
		destRect.h = 16;
		SDL_RenderCopy(renderer, spritePlayer, &srcRect, &destRect);

		PrintFont(renderClipRect.x, 24, "HELLO!");

		animTime++;
		if(animTime >= 12) {
			animTime = 0;
		}

        i++;
        if(i >= renderClipRect.x + PLAYABLE_W) {
            i = renderClipRect.x - 16;
        }
    }

	FreeGraphics();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
