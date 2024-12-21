#ifndef GLOBALS_H
#define GLOBALS_H

#include "raylib.h"

#include <string>
#include <cstddef>
#include <level/level.h>

inline int player_score = 0;

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

/* Shaders */

inline Shader crt_shader;
inline Shader air_shader;

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

void draw_image(Texture2D image, Vector2 pos, float width, float height);
void draw_image(Texture2D image, Vector2 pos, float size);

void load_sounds();
void unload_sounds();

#endif // GLOBALS_H