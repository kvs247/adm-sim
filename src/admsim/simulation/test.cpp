#include <gtest/gtest.h>
#include <vector>

#include "common/types.hpp"
#include "simulation.hpp"

TEST(CalculateWeights, EvenDistribution)
{
  const std::vector<VectorDataItem> vectorData{};
  const VelocimeterLocations velocimeterLocations{{{1, 1}, {1, -1}, {-1, 1}}};
  Simulation simulation(vectorData, velocimeterLocations);
  SimulationTester simulationTester(simulation);

  const std::vector<float> expectedWeights = {1.0f / 3, 1.0f / 3, 1.0f / 3};
  const auto actualWeights = simulationTester.testCalculateWeights(0, 0);

  ASSERT_EQ(expectedWeights, actualWeights);
}

TEST(CalculateWeights, UnevenDistribution)
{
  const std::vector<VectorDataItem> vectorData{};
  const VelocimeterLocations velocimeterLocations{{{1, 0}, {-2, 0}}};
  Simulation simulation(vectorData, velocimeterLocations);
  SimulationTester simulationTester(simulation);

  const std::vector<float> expectedWeights = {2.0f / 3, 1.0f / 3};
  const auto actualWeights = simulationTester.testCalculateWeights(0, 0);

  ASSERT_EQ(expectedWeights, actualWeights);
}