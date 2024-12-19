//
// Created by Timur Turatbekov on 29.11.2024.
//

#include "utilities.h"

#include <cstdlib>
#include <string>

float rand_from_to(float from, float to) {
    return from + static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX) * (to - from); // NOLINT(*-msc50-cpp)
}

float rand_up_to(float to) {
    return rand_from_to(0.0f, to);
}

Color get_color_from_hex(const std::string& hex_string) {
    std::string pure_hex_string;
    if (hex_string[0] == '#') {
        pure_hex_string = hex_string.substr(1);
    } else {
        pure_hex_string = hex_string;
    }

    if (pure_hex_string.size() == 6) {
        pure_hex_string += "FF";
    }

    unsigned int hex_value = stoul(pure_hex_string, nullptr, 16);
    return GetColor(hex_value);
}

Vector2i vector2i_add(Vector2i v1, Vector2i v2) {
    return { v1.x + v2.x, v1.y + v2.y };
}
