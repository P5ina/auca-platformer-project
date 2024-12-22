//
// Created by Timur Turatbekov on 20.12.2024.
//

#include "knowledge_book.h"

#include <nlohmann/json.hpp>
#include <utilities.h>

void to_json(nlohmann::json& j, const KnowledgeBook& b) {
    j = nlohmann::json {
        { "position", b.position },
        { "dialogue_lines", b.dialogue_lines }
    };
}

void from_json(const nlohmann::json& j, KnowledgeBook& b) {
    j.at("position").get_to(b.position);
    j.at("dialogue_lines").get_to(b.dialogue_lines);
}

void init_knowledge_book(b2WorldId world_id, KnowledgeBook* book) {
    b2BodyDef body_def = b2DefaultBodyDef();
    b2BodyId body_id = b2CreateBody(world_id, &body_def);

    b2Polygon box = b2MakeBox(0.5, 0.5);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.isSensor = true;
    shape_def.filter.categoryBits = PhysicsLayers::COLLECTABLE_LAYER;
    shape_def.filter.maskBits = PhysicsLayers::PLAYER_LAYER;
    shape_def.userData = book;

    b2CreatePolygonShape(body_id, &shape_def, &box);
    book->body_id = body_id;
}

