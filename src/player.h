#ifndef PLAYER_H
#define PLAYER_H

#include <global_state.h>

void spawn_player(std::unique_ptr<GameState> &game_state);
void move_player_horizontally(std::unique_ptr<GameState> &game_state, float delta);
void update_player(std::unique_ptr<GameState> &game_state);

#endif // PLAYER_H
