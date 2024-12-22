//
// Created by timat on 23.12.2024.
//

#ifndef MAGNETIC_BOX_H
#define MAGNETIC_BOX_H
#include <nlohmann/json.hpp>

#include "raylib.h"
#include "box2d/box2d.h"

struct MagneticBox {
    b2BodyId body_id;
    Vector2 spawn_position;
};

void to_json(nlohmann::json& j, const MagneticBox& b);
void from_json(const nlohmann::json& j, MagneticBox& b);

void init_magnetic_box(b2WorldId world_id, MagneticBox* magnetic_box);

#endif //MAGNETIC_BOX_H
