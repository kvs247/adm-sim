#pragma once

#include <vector>

#include "common/config.hpp"

struct Parcel
{
  float t;
  float x;
  float y;
  float sx;
  float sy;

  Parcel() = delete;
};

using ParcelLife = std::vector<Parcel>;