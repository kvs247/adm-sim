#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include "common/types.hpp"
#include "config.hpp"
#include "types.hpp"

class Simulation
{
public:
  Simulation() = delete;
  Simulation(const std::vector<VectorDataItem> &vd, const std::vector<XYVector> &vl)
      : vectorData(vd), velocimeterLocations(vl)
  {
  }

  std::vector<ParcelLife> runSimulation() const;

private:
  const std::vector<VectorDataItem> &vectorData;
  const std::vector<XYVector> &velocimeterLocations;

  XYVector calculateVector(const float x, const float y, const VectorDataItem &vectorData) const;
  std::vector<float> calculateWeights(const float x, const float y) const;
};