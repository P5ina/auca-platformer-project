//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "menu.h"
#include "raylib.h"

float main_menu_elapsed_time = 0.0f;

struct FireParticle {
    Vector2 position;
    float rotation;
    float rotationSpeed;
    float wobbleAmplitude;
    float wobbleFrequency;
    Color color;
};

void draw_menu() {
    main_menu_elapsed_time += GetFrameTime();
}

void draw_game_title() {

}