//
// Created by Timur Turatbekov on 21.12.2024.
//

#ifndef METRICS_H
#define METRICS_H
#include <global_state.h>

#include "raylib.h"

const float VIRTUAL_WIDTH = 640.0f;
const float VIRTUAL_HEIGHT = 360.0f;

const int INITIAL_WIDTH = 1280;
const int INITIAL_HEIGHT = 720;

enum UIAnchor {
    TOP_LEFT_ANCHOR,
    TOP_ANCHOR,
    TOP_RIGHT_ANCHOR,
    LEFT_ANCHOR,
    CENTER_ANCHOR,
    RIGHT_ANCHOR,
    BOTTOM_LEFT_ANCHOR,
    BOTTOM_ANCHOR,
    BOTTOM_RIGHT_ANCHOR
};

int get_display_width();
int get_display_height();

float get_ui_scale();
float get_ui_element_transform(UIAnchor anchor, Vector2 offset, Vector2 size);

float get_tile_screen_size(const Level* level);
Vector2 get_tile_screen_position(Level* level, Vector2 position);

#endif //METRICS_H
