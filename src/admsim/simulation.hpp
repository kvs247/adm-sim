#include <cmath>
#include <vector>
#include <iostream>

#include "common/types.hpp"
#include "config.hpp"
#include "types.hpp"

XYVector calculateVector(
    const float x,
    const float y,
    const VectorDataItem &vectorData,
    const std::vector<XYVector> &velocimeterLocations);

std::vector<ParcelLife>
simulation(const std::vector<VectorDataItem> &vectorData, const std::vector<XYVector> &velocimeterLocations)
{
  std::cout << "go\n";
  std::vector<ParcelLife> res;

  float t = 0;
  for (auto &vectors : vectorData)
  {
    for (auto &parcelLife : res)
    {
      const auto lastParcel = parcelLife.back();
      const auto currentVector = calculateVector(lastParcel.x, lastParcel.y, vectors, velocimeterLocations);

      const auto newT = lastParcel.t + 1;
      const auto newX = lastParcel.x + currentVector.x;
      const auto newY = lastParcel.y + currentVector.y;
      const auto newSx = lastParcel.sx * DISPERSION_GROWTH_COEFFICIENT;
      const auto newSy = lastParcel.sy * DISPERSION_GROWTH_COEFFICIENT;

      parcelLife.push_back({newT, newX, newY, newSx, newSy});
    }

    ParcelLife parcelLife{Parcel{t}};
    res.push_back(parcelLife);
    ++t;
  }

  return res;
}

XYVector calculateVector(
    const float x,
    const float y,
    const VectorDataItem &vectorData,
    const std::vector<XYVector> &velocimeterLocations)
{
  std::vector<float> weights;
  float total = 0;
  for (auto &location : velocimeterLocations)
  {
    const auto distance = std::hypot(x - location.x, y - location.y);
    const auto inverseDistance = std::pow(distance, -1);
    weights.push_back(inverseDistance);
    total += inverseDistance;
  }

  for (size_t i = 0; i < weights.size(); ++i)
  {
    weights[i] /= total;
  }

  XYVector res{0, 0};
  for (size_t i = 0; i < velocimeterLocations.size(); ++i)
  {
    res.x += weights[i] * vectorData[i].x;
    res.y += weights[i] * vectorData[i].y;
  }

  return res;
}