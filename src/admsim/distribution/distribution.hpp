#pragma once

#include <math.h>
#include <vector>

#include "../types.hpp"

class Distribution
{
public:
  Distribution() = delete;
  Distribution(const std::vector<ParcelLife> &sd) : simulationData(sd) {};

  std::vector<float> calculateDistributions() const;

private:
  const std::vector<ParcelLife> &simulationData;

  float calculateDensityAtXYT(const float x, const float y, const float t) const;

  std::vector<float> makeLocations() const;
};
