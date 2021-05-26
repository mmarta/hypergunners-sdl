#include "defines.h"
#include "graphics.h"
#include "background.h"

int main(int argc, char *argv[]) {
    u16 i = 0;

    // Begin
    SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

	SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "2", SDL_HINT_OVERRIDE);

    // Create the video
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	window = SDL_CreateWindow("HyperFire Dual Gunners '83",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  mode.w, mode.h, WINDOW_FLAGS);
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

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// Size to screen!
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
	SDL_RenderSetLogicalSize(renderer, PLAYABLE_W, PLAYABLE_H);

    // Load Graphics & Crosshatch
	if(LoadGraphics() != 0) {
		printf("Could not create graphics.");
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
	}

	BackgroundInit();

	DrawCrossHatchAndWait();

	// GO!
	const u8 *state = SDL_GetKeyboardState(NULL);

    while(!state[SDL_SCANCODE_ESCAPE]) {
        SDL_RenderPresent(renderer);

        SDL_PumpEvents(); // Needed for window events. Important!
        state = SDL_GetKeyboardState(NULL);

		// Updates
		BackgroundUpdate();

		// Draw
        ClearScreen();
		BackgroundDraw();
    }

	FreeGraphics();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
