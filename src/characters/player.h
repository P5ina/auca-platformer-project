//
// Created by Timur Turatbekov on 09.12.2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <global_state.h>
#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state);
void despawn_player(std::unique_ptr<GameState> &game_state);
void move_player_horizontally(std::unique_ptr<GameState> &game_state, float delta);
void update_player(std::unique_ptr<GameState> &game_state);
bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player);

#endif // PLAYER_H

