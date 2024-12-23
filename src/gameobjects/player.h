//
// Created by Timur Turatbekov on 09.12.2024.
//

#ifndef PLAYER_H
#define PLAYER_H

#include <global_state.h>
#include <input.h>

#include "box2d/box2d.h"

void spawn_player(std::unique_ptr<GameState> &game_state, Vector2 position, b2Vec2 initial_velocity);
void add_player_shape(b2BodyId body_id, b2Vec2 offset);
void despawn_player(std::unique_ptr<GameState> &game_state);

void update_player(GameState *game_state, GameInput *game_input, float delta);
bool is_grounded(b2WorldId world_id, std::unique_ptr<Player> &player);
void jump_player(GameState *game_state);

void update_magnet_player(Player* player, Level* level);
void update_magnet_object(Player* player, Level* level);
void update_magnet_cursor(Level *level, Player* player, GameInput *game_input);

Vector2i get_next_level_offset(const Level* level, const Player* player);
void clamp_player_position(const Level* level, const Player* player);
Vector2 get_next_level_player_position(const Level* level, const Player* player);

#endif // PLAYER_H

