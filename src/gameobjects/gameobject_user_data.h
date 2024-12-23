//
// Created by Timur Turatbekov on 22.12.2024.
//

#ifndef GAMEOBJECT_USER_DATA_H
#define GAMEOBJECT_USER_DATA_H

enum GameObjectType {
    GAME_OBJECT_PLAYER,
    GAME_OBJECT_KNOWLEDGE_BOOK,
    GAME_OBJECT_MAGNET_BLOCK,
    GAME_OBJECT_MAGNET_BOX,
    GAME_OBJECT_END_FLAG,
};

struct GameObjectUserData {
    GameObjectType type;
    int knowledge_book_id;
};

#endif //GAMEOBJECT_USER_DATA_H
