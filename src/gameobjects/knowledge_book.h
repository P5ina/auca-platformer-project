//
// Created by Timur Turatbekov on 20.12.2024.
//

#ifndef KNOWLEDGE_BOOK_H
#define KNOWLEDGE_BOOK_H

#include <string>
#include <vector>
#include <level/level.h>

#include "raylib.h"

const std::vector knowledge_books = {
    KnowledgeBook {
        .level_position = { 0, 0, 0 },
        .position = { 8.5f, 4.0f },
        .tip_text = "Use WASD to move."
    },
    KnowledgeBook {
        .level_position = { 1, 0, 0 },
        .position = { 8.5f, 4.0f },
        .tip_text = "Use WASD to move."
    },
};

#endif //KNOWLEDGE_BOOK_H
