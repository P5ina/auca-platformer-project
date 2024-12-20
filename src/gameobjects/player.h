//
// Created by Timur Turatbekov on 09.12.2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <global_state.h>
#include <input.h>

#include "box2d/box2d.h"

struct PlayerSpawn {
};

void spawn_player(std::unique_ptr<GameState> &game_state, Vector2 position);
void despawn_player(std::unique_ptr<GameState> &game_state);
void update_player(GameState *game_state, GameInput *game_input, float delta);
bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player);
void jump_player(GameState *game_state);

#endif // PLAYER_H

