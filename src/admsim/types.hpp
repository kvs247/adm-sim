#pragma once

#include <vector>

#include "config.hpp"

struct Parcel {
  float t = 0;
  float x = PARCEL_ORIGIN.x;
  float y = PARCEL_ORIGIN.y;
  float sx = INITIAL_PARCEL_DISPERSION;
  float sy = INITIAL_PARCEL_DISPERSION;
};

using ParcelLife = std::vector<Parcel>;