#pragma once

#include <array>

#include "config.hpp"

struct XYVector
{
  float x;
  float y;
};

using VelocimetersItem = std::array<XYVector, N_VELOCIMETERS>;
