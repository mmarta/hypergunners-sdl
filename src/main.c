#include "defines.h"
#include "system.h"
#include "graphics.h"
#include "background.h"
#include "bullet.h"
#include "player.h"

u8 frameTime = 0;

int main(int argc, char *argv[]) {
    u16 i = 0;

    // Begin
    SDL_Init(SDL_INIT_VIDEO);
	SDL_ShowCursor(SDL_DISABLE);

	//SDL_SetHintWithPriority(SDL_HINT_RENDER_SCALE_QUALITY, "0", SDL_HINT_OVERRIDE);

    // Create the video
	SDL_DisplayMode mode;
	SDL_GetDesktopDisplayMode(0, &mode);
	window = SDL_CreateWindow("HyperFire Dual Gunners '83",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							  SCREEN_W ? SCREEN_W : mode.w, SCREEN_H ? SCREEN_H : mode.h, WINDOW_FLAGS);
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

	PlayerInit(0);
	PlayerInit(1);

	// Go!
	ReadAllControls();

    while(!controlKeys[SDL_SCANCODE_ESCAPE]) {
        SDL_RenderPresent(renderer);

		// Input
		ReadAllControls();
		PlayerInput(0);
		PlayerInput(1);

		// Updates
		BackgroundUpdate();

		i = 0;
	    while(i < PLAYER_BULLET_TOTAL) {
	        PlayerBulletUpdate(i);
	        i++;
	    }

	    i = 0;
	    while(i < SHRAPNEL_BULLET_TOTAL) {
			ShrapnelBulletUpdate(i);
	        i++;
	    }

	    /*i = 0;
	    while(i < enemies.length) {
	        enemies[i].update();
	        i++;
	    }*/

	    PlayerUpdate(0);
		PlayerUpdate(1);

		// Draw
        ClearScreen();
		BackgroundDraw();

		i = 0;
	    while(i < PLAYER_BULLET_TOTAL) {
	        PlayerBulletDraw(i);
	        i++;
	    }

	    i = 0;
	    while(i < SHRAPNEL_BULLET_TOTAL) {
	        ShrapnelBulletDraw(i);
	        i++;
	    }

	    /*i = 0;
	    while(i < enemies.length) {
	        enemies[i].draw();
	        i++;
	    }*/

	    if(frameTime >= 2) {
	        PlayerDraw(1);
	        PlayerDraw(0);
	    } else {
			PlayerDraw(0);
	        PlayerDraw(1);
	    }
    }

	FreeGraphics();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
