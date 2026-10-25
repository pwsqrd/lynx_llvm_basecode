#include "game.h"

u16 g_levelTicks;

static u8 g_startHeld;

/*
=================
game::ResetStart
=================
*/
void game::ResetStart()
{
    g_startHeld = 1;
}

/*
=================
game::StartPressed
=================
*/
bool game::StartPressed()
{
    bool down = (SUZY.joystick & (Joystick::INNER | Joystick::OUTER)) || (SUZY.switches & Switches::PAUSE);
    if (!down) {
        g_startHeld = 0;
        return false;
    }
    if (g_startHeld) {
        return false;
    }
    g_startHeld = 1;
    return true;
}

/*
=================
main
=================
*/
int main(void)
{
    sys::Init();

    g_levelTicks = 0;
    g_startHeld  = 1;

	// This will jump to: ovl_Menu(void)
    u8 next = OVERLAY_MENU;
    while (1) {
        next = sys::overlay::Run(next);
    }
}
