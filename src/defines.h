#ifndef _DEFINES_H
#define _DEFINES_H

#include <SDL/SDL.h>
#include <stdio.h>

#define SCREEN_W 400
#define SCREEN_H 300

#define PLAYABLE_W 224
#define PLAYABLE_H 256

#if __APPLE__
    #define VIDEO_FLAGS SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_NOFRAME
#else
    #define VIDEO_FLAGS SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

#endif
