#include "system.h"

const u8 *controlKeys;

void ReadAllControls() {
    SDL_PumpEvents(); // Needed for window events. Important!
    controlKeys = SDL_GetKeyboardState(NULL);
}
