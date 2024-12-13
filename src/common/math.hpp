#pragma once

#include <math.h>

#include "types.hpp"

double degToRad(double d) { return d * M_PI / 180; }

XYVector polarToCart(float r, float t) { return {r * std::cos(t), r * std::sin(t)}; }

