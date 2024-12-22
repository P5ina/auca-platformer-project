//
// Created by Timur Turatbekov on 22.12.2024.
//

#ifndef GAMEOBJECT_USER_DATA_H
#define GAMEOBJECT_USER_DATA_H
#include <global_state.h>

enum GameObjectType {
    GAME_OBJECT_PLAYER,
    GAME_OBJECT_KNOWLEDGE_BOOK,
};

struct GameObjectUserData {
    GameObjectType type;
    int knowledge_book_id;
};

#endif //GAMEOBJECT_USER_DATA_H
