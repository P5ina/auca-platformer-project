//
// Created by timat on 23.12.2024.
//

#include "magnetic_box.h"

#include <nlohmann/json.hpp>
#include <utilities.h>

#include "gameobject_user_data.h"

void to_json(nlohmann::json& j, const MagneticBox& b) {
    j = nlohmann::json {
        { "position", b.spawn_position }
    };
}

void from_json(const nlohmann::json& j, MagneticBox& b) {
    j.at("position").get_to(b.spawn_position);
}

void init_magnetic_box(b2WorldId world_id, MagneticBox* magnetic_box) {
    b2BodyDef body_def = b2DefaultBodyDef();
    body_def.type = b2_dynamicBody;
    body_def.position = b2Vec2 {
        magnetic_box->spawn_position.x, magnetic_box->spawn_position.y
    };
    b2BodyId body_id = b2CreateBody(world_id, &body_def);

    b2Polygon box = b2MakeBox(0.5, 0.5);
    b2ShapeDef shape_def = b2DefaultShapeDef();
    shape_def.density = 45.0f;
    shape_def.filter.categoryBits = PhysicsLayers::MAGNET_BLOCK_LAYER;
    shape_def.filter.maskBits = PhysicsLayers::PLAYER_LAYER | PhysicsLayers::WALL_LAYER | PhysicsLayers::MAGNET_BLOCK_LAYER;
    shape_def.userData = new GameObjectUserData {
        .type = GAME_OBJECT_MAGNET_BOX,
    };

    b2CreatePolygonShape(body_id, &shape_def, &box);
    magnetic_box->body_id = body_id;
}
