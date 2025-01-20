#include <vector>

#include "../types.hpp"
#include "common/config.hpp"
#include "common/types.hpp"
#include "simulation.hpp"

std::vector<ParcelLife> Simulation::runSimulation() const
{
  std::vector<ParcelLife> res;

  const auto &config = Config::getInstance().getConfig();

  float t = 0;
  for (auto &vectors : vectorData)
  {
    for (auto &parcelLife : res)
    {
      const auto lastParcel = parcelLife.back();
      const auto currentVector = calculateVector(lastParcel.x, lastParcel.y, vectors);

      const auto newT = lastParcel.t + 1;
      const auto newX = lastParcel.x + currentVector.x * config.timestep * 1.5f;
      const auto newY = lastParcel.y + currentVector.y * config.timestep * 1.5f;
      const auto newSx = lastParcel.sx * config.dispersionGrowthCoefficient;
      const auto newSy = lastParcel.sy * config.dispersionGrowthCoefficient;

      parcelLife.push_back({newT, newX, newY, newSx, newSy});
    }

    ParcelLife parcelLife{Parcel{t, 0, 0, config.initialParcelDispersion, config.initialParcelDispersion}};
    res.push_back(parcelLife);
    ++t;
  }

  return res;
}

XYVector Simulation::calculateVector(const float x, const float y, const VectorDataItem &vectorData) const
{
  std::vector<float> weights = calculateWeights(x, y);

  XYVector res{0, 0};
  for (size_t i = 0; i < velocimeterLocations.size(); ++i)
  {
    res.x += weights[i] * vectorData[i].x;
    res.y += weights[i] * vectorData[i].y;
  }

  return res;
}

std::vector<float> Simulation::calculateWeights(const float x, const float y) const
{
  std::vector<float> res;
  float total = 0;
  for (auto &location : velocimeterLocations)
  {
    const auto distance = std::hypot(x - location.x, y - location.y);
    const auto inverseDistance = std::pow(distance, -1);
    res.push_back(inverseDistance);
    total += inverseDistance;
  }

  for (size_t i = 0; i < res.size(); ++i)
  {
    res[i] /= total;
  }

  return res;
};