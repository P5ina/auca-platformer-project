//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "animation.h"
#include "raymath.h"

float get_animation_value(float time, float duration, float delay) {
    float normalized_value = Normalize(time, delay, delay + duration);
    return Clamp(normalized_value, 0.0f, 1.0f);
}

float ease_out_sine(const float x) {
    return sinf(x * PI / 2);
}
