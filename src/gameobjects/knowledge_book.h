//
// Created by Timur Turatbekov on 20.12.2024.
//

#ifndef KNOWLEDGE_BOOK_H
#define KNOWLEDGE_BOOK_H

#include <vector>
#include <nlohmann/json.hpp>

#include "raylib.h"
#include "box2d/box2d.h"

struct KnowledgeBook {
    int id;
    bool collected = false;
    Vector2 spawn_position;
    std::vector<std::string> dialogue_lines;
    b2BodyId body_id;
};

void to_json(nlohmann::json& j, const KnowledgeBook& b);
void from_json(const nlohmann::json& j, KnowledgeBook& b);

void init_knowledge_book(b2WorldId world_id, KnowledgeBook* book);
void collect_knowledge_book(KnowledgeBook* book);

#endif //KNOWLEDGE_BOOK_H
