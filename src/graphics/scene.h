//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef SCENE_H
#define SCENE_H
#include <memory>

struct GameState;

enum class Scene {
    MENU_SCENE,
    LEVEL_SCENE,
    END_SCENE,
};

void draw_current_scene(const std::unique_ptr<GameState> &game_state);
void draw_placeholder_scene();

#endif //SCENE_H
