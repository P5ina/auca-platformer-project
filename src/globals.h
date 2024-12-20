#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#include <string>
#include <cstddef>
#include <level/level.h>

/* Player data */

const float GRAVITY_FORCE = 0.01f;
const float JUMP_STRENGTH = 0.3f;
const float MOVEMENT_SPEED = 0.1f;

inline int player_score = 0;

/* Graphic Metrics */

inline Vector2 screen_size;
inline float screen_scale; // Used to scale str/UI components size and displacements based on the screen_size size
inline float cell_size;
inline Vector2 shift_to_center;

/* Fonts */

inline Font menu_font;

/* Display Text Parameters */

struct Text {
    std::string str;
    Vector2 position = { 0.50f, 0.50f };
    float size = 32.0f;
    Color color = WHITE;
    float spacing = 4.0f;
    Font* font = &menu_font;
};

inline Text game_subtitle = {
    "Press Enter to Start",
    { 0.50f, 0.65f }
};

inline Text game_paused = {
    "Press Escape to Resume"
};

inline Text victory_title = {
    "You Won!",
    { 0.50f, 0.50f },
    100.0f,
    RED
};

inline Text victory_subtitle = {
    "Press Enter to go back to menu",
    { 0.50f, 0.65f }
};

/* Images and Sprites */

inline Texture2D wall_image;
inline Texture2D air_image;
inline Texture2D player_image;
inline Texture2D exit_image;

struct Sprite {
    size_t frame_count    = 0;
    size_t frames_to_skip = 3;
    size_t frames_skipped = 0;
    size_t frame_index    = 0;
    bool loop = true;
    size_t prev_game_frame = 0;
    Texture2D *frames = nullptr;
};

inline Sprite coin_sprite;

/* Shaders */

inline Shader crt_shader;

/* Sounds */

inline Sound coin_sound;
inline Sound exit_sound;
inline Sound jump_sound;

/* Victory Menu Background */

struct VictoryBall {
    float x, y;
    float dx, dy;
    float radius;
};

const size_t VICTORY_BALL_COUNT     = 2000;
const float VICTORY_BALL_MAX_SPEED  = 2.0f;
const float VICTORY_BALL_MIN_RADIUS = 2.0f;
const float VICTORY_BALL_MAX_RADIUS = 3.0f;
const Color VICTORY_BALL_COLOR      = { 180, 180, 180, 255 };
const unsigned char VICTORY_BALL_TRAIL_TRANSPARENCY = 10;
inline VictoryBall victory_balls[VICTORY_BALL_COUNT];

/* Frame Counter */

inline size_t game_frame = 0;

/* Forward Declarations */

// GRAPHICS_H

inline void derive_graphics_metrics_from_level(std::unique_ptr<Level> &level);
inline void draw_pause_menu();
inline void create_victory_menu_background();
inline void animate_victory_menu_background();
inline void draw_victory_menu_background();
inline void draw_victory_menu();

// ASSETS_H

void load_shaders();
void unload_shaders();

void load_fonts();
void unload_fonts();

void load_images();
void unload_images();

void draw_image(Texture2D image, Vector2 pos, float width, float height);
void draw_image(Texture2D image, Vector2 pos, float size);

Sprite load_sprite(
    const std::string &file_name_prefix,
    const std::string &file_name_suffix,
    size_t frame_count = 1,
    bool loop = true,
    size_t frames_to_skip = 3
);
void unload_sprite(Sprite &sprite);
void draw_sprite(Sprite &sprite, Vector2 pos, float width, float height);
void draw_sprite(Sprite &sprite, Vector2 pos, float size);

void load_sounds();
void unload_sounds();

#endif // GLOBALS_H