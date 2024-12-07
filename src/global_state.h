//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef GLOBAL_STATE_H
#define GLOBAL_STATE_H
#include <memory>
#include <graphics/scene.h>
#include <level/level.h>

struct GameState {
    Scene scene = Scene::MENU_SCENE;
    std::unique_ptr<LoadedLevel> loaded_level;
    // Assets assets;
};

#endif //GLOBAL_STATE_H
