#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "defines.h"

#define SCREEN_W 400
#define SCREEN_H 300

#define PLAYABLE_W 224
#define PLAYABLE_H 256

#if __APPLE__
    #define WINDOW_FLAGS SDL_WINDOW_BORDERLESS
#else
    #define WINDOW_FLAGS SDL_WINDOW_FULLSCREEN
#endif

SDL_Window *window;
SDL_Renderer *renderer;

extern SDL_Texture *font;
extern SDL_Texture *spritePlayer;
extern SDL_Texture *spriteEnemy;

extern SDL_Rect srcRect, destRect;

SDL_Texture * LoadImage(const char *);
u8 LoadGraphics();
void FreeGraphics();
void PrintFont(int, int, const char *);

#endif