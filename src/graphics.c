#include "graphics.h"

SDL_Texture *font = NULL;
SDL_Texture *spritePlayer = NULL;
SDL_Texture *spriteEnemy = NULL;

SDL_Rect srcRect, destRect;

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
    font = LoadImage("img/font.png");
    if(!font) {
        FreeGraphics();
        return 1;
    }

    spritePlayer = LoadImage("img/player.png");
    if(!spritePlayer) {
        FreeGraphics();
        return 2;
    }

    spriteEnemy = LoadImage("img/enemy.png");
    if(!spriteEnemy) {
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

/*
const printFontNumber = function(x, y, val, maxVal) {
    let first = true, modVal, code;

    do {
        modVal = val % 10;
        code = modVal + 16;
        context.drawImage(tileFont, !first && modVal == 0 && val == 0 ? 0 : code << 3, 0, 8, 8, x, y, 8, 8);
        x -= 8;
        val = (val / 10) >> 0;
        maxVal = (maxVal / 10) >> 0;
        first = false;
    } while(maxVal > 0);
};

const drawCrossHatchWithCallback = function(mainCallback) {
    setTimeout(() => {
        let i;
        resetContext();
        context.fillStyle = '#fff';

        i = 0;
        while(i < 256) {
            context.fillRect(0, i, OVERSCAN_W, 1);
            i += 16;
        }

        i = 0;
        while(i < 224) {
            context.fillRect(i, 0, 1, DISPLAY_H);
            i += 16;
        }

        setTimeout(() => {
            mainCallback();
        }, 2000);

        flip();
    }, 1000);
};

const flip = function() {
    displayContext.drawImage(drawable, 8, 0);
};
*/
