#include "collision.h"

u8 HitboxCollision(Hitbox *hitbox1, Hitbox *hitbox2) {
    if(hitbox1->collidable && hitbox2->collidable) {
        if(
            hitbox1->rect.x < hitbox2->rect.w + hitbox2->rect.x
            && hitbox2->rect.x < hitbox1->rect.w + hitbox1->rect.x
            && hitbox1->rect.y < hitbox2->rect.h + hitbox2->rect.y
            && hitbox2->rect.y < hitbox1->rect.h + hitbox1->rect.y
        ) {
            return 1;
        }
    }

    return 0;
};
