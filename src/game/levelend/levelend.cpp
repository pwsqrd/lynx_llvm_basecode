#include "game/game.h"
#include "cart_files_levelend.h"
#include "sys/suzy_math.h"

constexpr u8 BACKGROUND_PEN = 15;
constexpr u8 DIGIT_WIDTH    = 6;
constexpr u8 TICKS_PER_SEC  = 60;

static u8  g_spritePool[1536];
static const sys::sprite::Sprite* g_sprites[LEVELEND_SPRITE_COUNT];

/*
=================
levelend_DrawNumber

Draws value right-aligned so its last digit ends at x. The divide runs on
Suzy while the CPU steps x; Get16 waits for the result.
=================
*/
static void levelend_DrawNumber(s16 x, s16 y, u16 value)
{
    do {
        auto div = sys::math::StartDivide(value, (u16)10);
        x -= DIGIT_WIDTH;
        u16 quotient = div.Get16();
        u8 digit = (u8)(value - quotient * 10);
        sys::sprite::Draw(x, y, g_sprites[LEVELEND_DIGIT_0_SPR_IDX + digit]);
        value = quotient;
    } while (value);
}

/*
=================
ovl_Main

Shows the level time, waits for start, then returns to the menu.
=================
*/
extern "C" u8 ovl_Main(void)
{
    u16 used = 0;
    for (u8 i = 0; i < LEVELEND_SPRITE_COUNT; i++) {
        u16 size = sys::cart::AssetSize(LEVELEND_SPRITES[i]);
        g_sprites[i] = reinterpret_cast<const sys::sprite::Sprite*>(g_spritePool + used);
        sys::cart::Read(g_spritePool + used, LEVELEND_SPRITES[i]);
        used += size;
    }

    u16 seconds = sys::math::StartDivide(g_levelTicks, (u16)TICKS_PER_SEC).Get16();

    sys::SetPalette(LEVELEND_PAL);
    sys::SetPaletteColor(BACKGROUND_PEN, 0, 3, 1);

    game::ResetStart();
    while (1) {
        sys::StartFrame();
        sys::ClearScreen(BACKGROUND_PEN);

        sys::sprite::Draw((SYS_SCREEN_WIDTH - g_sprites[LEVELEND_LEVEL_COMPLETE_SPR_IDX]->width) / 2, 26, g_sprites[LEVELEND_LEVEL_COMPLETE_SPR_IDX]);

        sys::sprite::Draw(56, 46, g_sprites[LEVELEND_TIME_SPR_IDX]);
        levelend_DrawNumber(104, 46, seconds);

        if (sys::GetTicks() & 0x20) {
            sys::sprite::Draw((SYS_SCREEN_WIDTH - g_sprites[LEVELEND_PRESS_START_SPR_IDX]->width) / 2, 70, g_sprites[LEVELEND_PRESS_START_SPR_IDX]);
        }

        bool start = game::StartPressed();
        sys::EndFrame();

        if (start) {
            return OVERLAY_MENU;
        }
    }
}
