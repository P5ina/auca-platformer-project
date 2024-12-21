//
// Created by Timur Turatbekov on 20.12.2024.
//

#ifndef DIALOGUE_H
#define DIALOGUE_H
#include <assets.h>
#include <input.h>
#include <vector>

struct Dialogue {
    std::vector<std::string> lines;
    float last_update = 0;
    int current_line = 0;
    int current_line_progress = 0;
};

void update_dialogue(Dialogue *dialogue, GameInput* game_input, Assets* assets);
bool is_dialogue_ended(const Dialogue *dialogue);
void draw_dialogue(Dialogue *dialogue, Assets* assets);

#endif //DIALOGUE_H
