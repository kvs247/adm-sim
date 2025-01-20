#pragma once

struct XYVector
{
  float x;
  float y;
};

static constexpr size_t MAX_VELOCIMETERS = 16;
using VectorDataItem = std::array<XYVector, MAX_VELOCIMETERS>;

using VelocimeterLocations = std::vector<XYVector>;
