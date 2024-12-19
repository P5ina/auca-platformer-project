//
// Created by Timur Turatbekov on 09.12.2024.
//

#ifndef INPUT_H
#define INPUT_H

struct GameInput {
    float horizontal_movement;
    bool jump;
    bool toggle_debug_mode;
};

GameInput read_game_input();

#endif //INPUT_H
