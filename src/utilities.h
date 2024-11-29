#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include "raylib.h"

float rand_from_to(float from, float to);
float rand_up_to(float to);
Color get_color_from_hex(const std::string& hex);

#endif // UTILITIES_H
