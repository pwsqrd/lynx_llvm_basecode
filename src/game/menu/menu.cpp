#include "game/game.h"
#include "cart_files_menu.h"

constexpr u8 BACKGROUND_PEN = 15;

static u8  g_spritePool[1024];
static const sys::sprite::Sprite* g_sprites[MENU_SPRITE_COUNT];

/*
=================
ovl_Main

Title screen: waits for start, then runs the level.
=================
*/
extern "C" u8 ovl_Main(void)
{
    u16 used = 0;
    for (u8 i = 0; i < MENU_SPRITE_COUNT; i++) {
        u16 size = sys::cart::AssetSize(MENU_SPRITES[i]);
        g_sprites[i] = reinterpret_cast<const sys::sprite::Sprite*>(g_spritePool + used);
        sys::cart::Read(g_spritePool + used, MENU_SPRITES[i]);
        used += size;
    }

    sys::SetPalette(MENU_PAL);
    sys::SetPaletteColor(BACKGROUND_PEN, 1, 1, 4);

    game::ResetStart();
    while (1) {
        sys::StartFrame();
        sys::ClearScreen(BACKGROUND_PEN);

        sys::sprite::Draw((SYS_SCREEN_WIDTH - g_sprites[MENU_TITLE_SPR_IDX]->width) / 2, 28, g_sprites[MENU_TITLE_SPR_IDX]);
        if (sys::GetTicks() & 0x20) {
            sys::sprite::Draw((SYS_SCREEN_WIDTH - g_sprites[MENU_PRESS_START_SPR_IDX]->width) / 2, 66, g_sprites[MENU_PRESS_START_SPR_IDX]);
        }

        bool start = game::StartPressed();
        sys::EndFrame();

        if (start) {
            return OVERLAY_LEVEL;
        }
    }
}
