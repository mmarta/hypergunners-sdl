#include "defines.h"
#include "system.h"
#include "graphics.h"
#include "background.h"
#include "bullet.h"
#include "player.h"
#include "enemy.h"

u8 frameTime = 0;

int main(int argc, char *argv[]) {
    u16 i = 0;
	u8 j, k;

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

	PlayerInit(&players[0], 0);
	//PlayerInit(1);
	EnemyInitAll();

	// Go!
	ReadAllControls();

    while(!controlKeys[SDL_SCANCODE_ESCAPE]) {
        SDL_RenderPresent(renderer);

		// Input
		ReadAllControls();
		PlayerInput(&players[0]);
		PlayerInput(&players[1]);

		// Check collisions first
	    i = 0;
	    while(i < ENEMY_TOTAL) {
            // Player Collisions First

	        j = 0;
	        while(j < PLAYER_TOTAL) {
	            if(HitboxCollision(&enemies[i].hitbox, &players[j].hitbox)) {
	                EnemyKill(&enemies[i], &players[j], 1);
	                PlayerKill(&players[j]);
	                break;
	            } else if(HitboxCollision(&enemies[i].hitbox, &players[j].clawLine.hitbox)) {
                    EnemyGrab(&enemies[i], &players[j]);
                    ClawLineReturn(&players[j].clawLine, 1);
                    break;
                }

                k = 0;
    	        while(k < PLAYER_BULLET_TOTAL) {
    	            if(HitboxCollision(&enemies[i].hitbox, &players[j].bullets[k].hitbox)) {
    	                EnemyKill(&enemies[i], &players[j], 1);
    	                PlayerBulletDeactivate(&players[j].bullets[k]);
    	                break;
    	            }
    	            k++;
    	        }

    	        k = 0;
    	        while(k < MULTIPLE_BULLET_TOTAL) {
    	            if(HitboxCollision(&enemies[i].hitbox, &players[j].multiple.bullets[k].hitbox)) {
    	                EnemyKill(&enemies[i], &players[j], 1);
    	                PlayerBulletDeactivate(&players[j].multiple.bullets[k]);
    	                break;
    	            }
    	            k++;
    	        }

	            j++;
	        }

            // Then shrapnel collision
            j = 0;
            while(j < SHRAPNEL_BULLET_TOTAL) {
                if(HitboxCollision(&enemies[i].hitbox, &shrapnelBullets[j].hitbox)) {
                    EnemyKill(&enemies[i], &players[shrapnelBullets[j].associatedPlayerIndex], shrapnelBullets[j].multiplier);
                    ShrapnelBulletDeactivate(&shrapnelBullets[j]);
                    break;
                }
                j++;
            }

	        i++;
	    }

        i = 0;
        while(i < PLAYER_TOTAL) {
            // Then shrapnel collision
            j = 0;
	        while(j < SHRAPNEL_BULLET_TOTAL) {
	            if(HitboxCollision(&players[i].hitbox, &shrapnelBullets[j].hitbox)) {
	                PlayerKill(&players[i]);
	                ShrapnelBulletDeactivate(&shrapnelBullets[j]);
	                break;
	            }
	            j++;
	        }

            i++;
        }

		// Updates
		BackgroundUpdate();

		if(rand() % 10 == 0) {
	        EnemySpawnNext();
	    }

	    i = 0;
	    while(i < SHRAPNEL_BULLET_TOTAL) {
			ShrapnelBulletUpdate(&shrapnelBullets[i]);
	        i++;
	    }

	    i = 0;
	    while(i < ENEMY_TOTAL) {
	        EnemyUpdate(&enemies[i]);
	        i++;
	    }

	    PlayerUpdate(&players[0]);
		PlayerUpdate(&players[1]);

		// Draw
        ClearScreen();
		BackgroundDraw();

	    i = 0;
	    while(i < SHRAPNEL_BULLET_TOTAL) {
	        ShrapnelBulletDraw(&shrapnelBullets[i]);
	        i++;
	    }

		i = 0;
	    while(i < ENEMY_TOTAL) {
	        EnemyDraw(&enemies[i]);
	        i++;
	    }

	    if(frameTime >= 2) {
	        PlayerDraw(&players[1]);
	        PlayerDraw(&players[0]);
	    } else {
			PlayerDraw(&players[0]);
	        PlayerDraw(&players[1]);
	    }
    }

	FreeGraphics();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
