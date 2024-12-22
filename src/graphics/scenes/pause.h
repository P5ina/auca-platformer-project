//
// Created by Timur Turatbekov on 22.12.2024.
//

#ifndef PAUSE_H
#define PAUSE_H
#include <assets.h>
#include <input.h>
#include <optional>
#include <graphics/scene.h>

struct PauseMenuState {
    bool is_paused;
    int selected_option;
    std::optional<Scene> next_scene;
};

enum PauseMenuOption {
    PAUSE_OPTION_CONTINUE = 0,
    PAUSE_OPTION_EXIT = 1,
};

void draw_pause_menu(PauseMenuState *state, Assets *assets);
void draw_pause_menu_option(PauseMenuOption option, PauseMenuState *state, Assets *assets);

void update_pause_menu(PauseMenuState *state, GameInput *game_input);

#endif //PAUSE_H
