#include "graphics.h"

SDL_Texture *font = NULL;
SDL_Texture *spritePlayer = NULL;
SDL_Texture *spriteEnemy = NULL;

SDL_Rect srcRect, destRect, renderClipRect;
u8 imageInit = 0;

SDL_Texture* LoadImage(const char *filename) {
	SDL_Surface *tempSurface = IMG_Load(filename);
	if(!tempSurface) {
	    return NULL;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if(!texture) {
		SDL_FreeSurface(tempSurface);
		return NULL;
	}

	return texture;
}

u8 LoadGraphics() {
	int initted = IMG_Init(IMG_INIT_PNG);
	if((initted & IMG_INIT_PNG) != IMG_INIT_PNG) {
		FreeGraphics();
		return 1;
	}

	imageInit = 1;

	font = LoadImage("img/font.png");
    if(!font) {
		printf("Error loading font: %s\n", IMG_GetError());
        FreeGraphics();
        return 1;
    }

    spritePlayer = LoadImage("img/player.png");
    if(!spritePlayer) {
		printf("Error loading player sprite: %s\n", IMG_GetError());
        FreeGraphics();
        return 2;
    }

    spriteEnemy = LoadImage("img/enemy.png");
    if(!spriteEnemy) {
		printf("Error loading enemy sprite: %s\n", IMG_GetError());
        FreeGraphics();
        return 2;
    }

    return 0;
}

void FreeGraphics() {
    if(font) {
        SDL_DestroyTexture(font);
    }

    if(spritePlayer) {
        SDL_DestroyTexture(spritePlayer);
    }

    if(spriteEnemy) {
        SDL_DestroyTexture(spriteEnemy);
    }

	if(imageInit) {
		IMG_Quit();
	}
}

void ClearScreen() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void PrintFont(int x, int y, const char *str) {
    u8 i = 0, code;
    while(*str) {
        code = *str - 32;

        srcRect.x = (code % 32) << 3;
        srcRect.y = code >= 32 ? 8 : 0;
        srcRect.w = 8;
        srcRect.h = 8;

        destRect.x = x;
        destRect.y = y;
        destRect.w = 8;
        destRect.h = 8;

        SDL_RenderCopy(renderer, font, &srcRect, &destRect);
        x += 8;
        str++;
    }
}

void PrintFontNumber(int x, int y, int val, int maxVal) {
    u8 first = 1, modVal, code;

    do {
        modVal = val % 10;
        code = modVal + 16;

        srcRect.x = !first && modVal == 0 && val == 0 ? 0 : code << 3;
        srcRect.y = 0;
        srcRect.w = 8;
        srcRect.h = 8;

        destRect.x = x;
        destRect.y = y;
        destRect.w = 8;
        destRect.h = 8;

        SDL_RenderCopy(renderer, font, &srcRect, &destRect);
        x -= 8;
        val = val / 10;
        maxVal = maxVal / 10;
        first = 0;
    } while(maxVal > 0);
}

void DrawCrossHatchAndWait() {
    int i, end, frames = 0;

    while(frames < 120) {
        SDL_RenderPresent(renderer);
        SDL_PumpEvents();

        ClearScreen();

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        destRect.x = renderClipRect.x;
        destRect.w = PLAYABLE_W;
        destRect.h = 1;

        i = renderClipRect.y;
        end = i + PLAYABLE_H;
        while(i < end) {
            destRect.y = i;
            SDL_RenderFillRect(renderer, &destRect);
            i += 16;
        }

        destRect.y = renderClipRect.y;
        destRect.w = 1;
        destRect.h = PLAYABLE_H;

        i = renderClipRect.x;
        end = i + PLAYABLE_W;
        while(i < end) {
            destRect.x = i;
            SDL_RenderFillRect(renderer, &destRect);
            i += 16;
        }

        frames++;
    }
}
