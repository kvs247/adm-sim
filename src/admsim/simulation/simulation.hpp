#pragma once

#include <cmath>
#include <iostream>
#include <vector>

#include "../config.hpp"
#include "../types.hpp"
#include "common/types.hpp"

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

  friend struct SimulationTester;
};

struct SimulationTester
{
public:
  SimulationTester(Simulation &s) : simulation(s) {}

  std::vector<float> testCalculateWeights(const float x, const float y) const
  {
    return simulation.calculateWeights(x, y);
  };

private:
  Simulation &simulation;
};