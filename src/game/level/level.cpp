#include "game/game.h"
#include "cart_files_level.h"
#include "sys/suzy_math.h"
#include "sys/chipper/chipper.h"
#include "sys/chipper/chipper_sfx.h"

extern "C" {
    extern const u8* music_demo_ptrs[4];
}

constexpr u8 BACKGROUND_PEN = 14;
constexpr u8 DOOR_PEN       = 15;
constexpr u8 FOLLOWER_PEN   = 13;
constexpr u8 SLIME_SIZE     = 16;
constexpr u8 DOOR_X         = 146;
constexpr u8 DOOR_Y         = 40;
constexpr u8 DOOR_W         = 10;
constexpr u8 DOOR_H         = 20;
constexpr s16 FOLLOW_FRAMES = 12;

static const u8 FOLLOWER_PENPAL[8] = { 0x01, 0xDD, 0xDD, 0xD7, 0xDD, 0xAB, 0xCD, 0xEF };

static u8  g_spritePool[1024];
static const sys::sprite::Sprite* g_slime[CHARACTER_SPRITE_COUNT];

static s16  g_x;
static s16  g_y;
static u8   g_facing;
static bool g_moving;
static u8   g_joy;
static u8   g_prevJoy;

/*
=================
level_LoadSprites
=================
*/
static void level_LoadSprites()
{
    u16 used = 0;
    for (u8 i = 0; i < CHARACTER_SPRITE_COUNT; i++) {
        u16 size = sys::cart::AssetSize(CHARACTER_SPRITES[i]);
        g_slime[i] = reinterpret_cast<const sys::sprite::Sprite*>(g_spritePool + used);
        sys::cart::Read(g_spritePool + used, CHARACTER_SPRITES[i]);
        used += size;
    }
}

/*
=================
level_HandleInput

Reads the joystick, moves the player and keeps it on screen.
=================
*/
static void level_HandleInput()
{
    g_joy = SUZY.joystick;
    g_moving = false;
    if (g_joy & Joystick::LEFT) {
        g_x -= 1;
        g_facing = CHARACTER_TG_MONSTERS_SLIME_L1_SPR_IDX;
        g_moving = true;
    }
    if (g_joy & Joystick::RIGHT) {
        g_x += 1;
        g_facing = CHARACTER_TG_MONSTERS_SLIME_R1_SPR_IDX;
        g_moving = true;
    }
    if (g_joy & Joystick::UP) {
        g_y -= 1;
        g_facing = CHARACTER_TG_MONSTERS_SLIME_U1_SPR_IDX;
        g_moving = true;
    }
    if (g_joy & Joystick::DOWN) {
        g_y += 1;
        g_facing = CHARACTER_TG_MONSTERS_SLIME_D1_SPR_IDX;
        g_moving = true;
    }

    if (g_x < 0) {
        g_x = 0;
    }
    if (g_x > SYS_SCREEN_WIDTH - SLIME_SIZE) {
        g_x = SYS_SCREEN_WIDTH - SLIME_SIZE;
    }
    if (g_y < 0) {
        g_y = 0;
    }
    if (g_y > SYS_SCREEN_HEIGHT - SLIME_SIZE) {
        g_y = SYS_SCREEN_HEIGHT - SLIME_SIZE;
    }
}

/*
=================
level_HandleSound

Plays music or a sound effect for buttons pressed this frame.
=================
*/
static void level_HandleSound()
{
    u8 pressed = g_joy & ~g_prevJoy;
    g_prevJoy = g_joy;
    if (pressed & Joystick::INNER) {
        sys::chipper::Play(0, music_demo_ptrs[0]);
    }
    if (pressed & Joystick::OUTER) {
        sys::chipper::Play(1, sfx_melody);
    }
    if (pressed & Joystick::OPTION1) {
        sys::chipper::Play(2, sfx_melody);
    }
}

/*
=================
ovl_Main

Walk the slime to the door, with a green slime following behind. A plays
music, B and OPTION1 play a sound effect.

Each frame the follower closes 1/FOLLOW_FRAMES of the gap on each axis. Those
two divides run on Suzy while the CPU handles input and sound.
=================
*/
extern "C" u8 ovl_Main(void)
{
    level_LoadSprites();

    sys::SetPalette(CHARACTER_PAL);
    sys::SetPaletteColor(BACKGROUND_PEN, 1, 2, 3);
    sys::SetPaletteColor(DOOR_PEN, 15, 13, 3);
    sys::SetPaletteColor(FOLLOWER_PEN, 3, 13, 5);

    g_x       = 8;
    g_y       = (SYS_SCREEN_HEIGHT - SLIME_SIZE) / 2;
    g_facing  = CHARACTER_TG_MONSTERS_SLIME_R1_SPR_IDX;
    g_prevJoy = SUZY.joystick;

    u16 fx    = (u16)g_x << 8;
    u16 fy    = (u16)g_y << 8;
    u16 start = sys::GetTicks();

    while (1) {
        sys::StartFrame();
        sys::ClearScreen(BACKGROUND_PEN);
        sys::ClearScreenRegion(DOOR_X, DOOR_Y, DOOR_W, DOOR_H, DOOR_PEN);

        s24 gapX = (s24)g_x * 256 - fx;
        s24 gapY = (s24)g_y * 256 - fy;

        auto divX = sys::math::StartDivide(gapX, FOLLOW_FRAMES);
        level_HandleInput();
        fx += (u16)divX.Get16();

        auto divY = sys::math::StartDivide(gapY, FOLLOW_FRAMES);
        level_HandleSound();
        fy += (u16)divY.Get16();

        u8 frame = g_facing;
        if (g_moving && (sys::GetTicks() & 0x08)) {
            frame++;
        }

        sys::sprite::SetPenpal4BPP(FOLLOWER_PENPAL);
        sys::sprite::Draw((s16)(fx >> 8), (s16)(fy >> 8), g_slime[frame]);
        sys::sprite::InitPenpal();
        sys::sprite::Draw(g_x, g_y, g_slime[frame]);

        sys::EndFrame();

        bool atDoor = g_x + SLIME_SIZE > DOOR_X && g_x < DOOR_X + DOOR_W
                   && g_y + SLIME_SIZE > DOOR_Y && g_y < DOOR_Y + DOOR_H;
        if (atDoor) {
            g_levelTicks = sys::GetTicks() - start;
            return OVERLAY_LEVELEND;
        }
    }
}
