#ifndef GAME_H
#define GAME_H

#include "sys/sys.h"
#include "sys/overlay.h"

// Resident state shared between overlays. Overlay memory is replaced on every
// switch, so anything one overlay hands to the next lives here.
extern u16 g_levelTicks;

namespace game {

// Edge-triggered start press (A, B or PAUSE). The press that ended the previous
// screen must be released first, so call ResetStart when a screen opens.
void ResetStart();
bool StartPressed();

}

#endif
