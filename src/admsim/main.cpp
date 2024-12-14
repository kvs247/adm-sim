#include <iostream>

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/types.hpp"
#include "distribution/distribution.hpp"
#include "simulation/simulation.hpp"
#include "types.hpp"

int main()
{
  std::cout << "admsim\n";

  const auto vectorData = readVector<VectorDataItem>(VECTOR_DATA_PATH);
  // for (auto &item : vectorData)
  // {
  //   std::cout << "{\n";
  //   for (auto j = 0; j < N_VELOCIMETERS; ++j)
  //   {
  //     std::cout << "\t" << j << ": {" << item[j].x << ", " << item[j].y << "},\n";
  //   }
  //   std::cout << "},\n";
  // }
  // std::cout << "\n";

  const auto velocimetersData = readVector<XYVector>(VELOCIMETER_LOCATIONS_PATH);
  // for (auto &item : velocimetersData)
  // {
  //   std::cout << "{" << item.x << ", " << item.y << "}\n";
  // }

  std::cout << "vectodata: " << vectorData.size() << "\n";

  Simulation simulation(vectorData, velocimetersData);
  const auto simulationData = simulation.runSimulation();

  for (const ParcelLife &x : simulationData)
  {
    std::cout << "new Parcel\n";
    for (const Parcel &y : x)
    {
      std::cout << "{" << y.t << ", " << y.x << ", " << y.y << ", " << y.sx << ", " << y.sy << "}\n";
    }
  }

  Distribution distribution(simulationData);
  auto distributionData = distribution.calculateDistributions();

  writeVector(distributionData, "distributionData");
}