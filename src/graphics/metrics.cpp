//
// Created by Timur Turatbekov on 21.12.2024.
//

#include "metrics.h"

#include <algorithm>
#include <globals.h>
#include <global_state.h>

#include "raylib.h"

int get_display_width() {
    if (IsWindowFullscreen()) {
        const int monitor = GetCurrentMonitor();
        return GetMonitorWidth(monitor);
    }
    return GetScreenWidth();
}

int get_display_height() {
    if (IsWindowFullscreen()) {
        const int monitor = GetCurrentMonitor();
        return GetMonitorHeight(monitor);
    }
    return GetScreenHeight();
}


float get_ui_scale() {
    int width = get_display_width();
    int height = get_display_height();

    return std::min(
        static_cast<float>(width) / VIRTUAL_WIDTH,
        static_cast<float>(height) / VIRTUAL_HEIGHT
    );
}

float get_tile_screen_size(const Level *level) {
    Vector2 screen_size;
    screen_size.x = static_cast<float>(get_display_width());
    screen_size.y = static_cast<float>(get_display_height());

    return std::min(
        screen_size.x / static_cast<float>(level->columns),
        screen_size.y / static_cast<float>(level->rows)
    );
}

Vector2 get_tile_screen_position(Level *level, Vector2 position) {
    auto screen_size = Vector2 {
        static_cast<float>(get_display_width()),
        static_cast<float>(get_display_height())
    };

    const float tile_size = get_tile_screen_size(level);

    const float level_width = static_cast<float>(level->columns) * tile_size;
    const float level_height = static_cast<float>(level->rows) * tile_size;
    auto shift_to_center = Vector2 {
        (screen_size.x - level_width) / 2.0f,
        (screen_size.y - level_height) / 2.0f
    };
    return Vector2 {
        shift_to_center.x + position.x * tile_size,
        shift_to_center.y + position.y * tile_size,
    };
}
