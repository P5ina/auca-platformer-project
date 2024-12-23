//
// Created by timat on 23.12.2024.
//

#ifndef BOSS_H
#define BOSS_H
#include "box2d/box2d.h"

struct Boss {
    b2Vec2 position1;
    b2Vec2 position2;
    b2Vec2 spawn_position;
    b2BodyId body_id;
    int health;
    bool go_left;
    float flip_time;
};

void update_boss(Boss* boss);

#endif //BOSS_H
