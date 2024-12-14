#pragma once

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
};
