//
// Created by Timur Turatbekov on 29.11.2024.
//

#ifndef SCENE_H
#define SCENE_H
#include <globals.h>

enum Scene {
    MENU_SCENE,
    LEVEL_SCENE,
    END_SCENE,
};

void draw_current_scene();
void draw_placeholder_scene();

#endif //SCENE_H
