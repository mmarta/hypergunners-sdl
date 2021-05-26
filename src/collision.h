#ifndef _COLLISION_H
#define _COLLISION_H

#include "defines.h"

typedef struct {
    SDL_Rect rect;
    u8 collidable;
} Hitbox;

u8 HitboxCollision(Hitbox *, Hitbox *);

#endif
