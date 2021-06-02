#include "system.h"

const u8 *controlKeys;

void ReadAllControls() {
    controlKeys = SDL_GetKeyboardState(NULL);
    SDL_PumpEvents(); // Needed for window events. Important!
}
