//
// Created by Timur Turatbekov on 09.12.2024.
//

#ifndef INPUT_H
#define INPUT_H

struct GameInput {
    float horizontal_movement;

    bool select_up_pressed;
    bool select_down_pressed;
    bool select_pressed;

    bool jump;
    bool toggle_debug_mode;
    bool toggle_full_screen;
    bool skip_dialogue_line;
    bool pause_pressed;
};

GameInput read_game_input();

#endif //INPUT_H
