#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include <random>
#include <vector>

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

const float targetDirectionDeg = 135;
const float directionSpreadDeg = 5.0f;
const float targetSpeed = 6;
const float speedSpreadPercent = 0.1f;
  
std::mt19937 randomGenerator(std::random_device{}());

std::vector<VectorDataItem> makeVectorData()
{

  // could also do uniform_real_distribution
  std::normal_distribution<float> rNoise(0.0f, targetSpeed * speedSpreadPercent);
  std::normal_distribution<float> tNoise(0.0f, degToRad(directionSpreadDeg));

  std::vector<VectorDataItem> res(N_READINGS);
  for (int i = 0; i < N_READINGS; ++i)
  {
    VectorDataItem resItem;
    for (int j = 0; j < N_VELOCIMETERS; ++j)
    {
      const float r = targetSpeed + rNoise(randomGenerator);
      const float t = degToRad(targetDirectionDeg) + tNoise(randomGenerator);
      resItem[j] = polarToCart(r, t);
    }
    res[i] = resItem;
  }

  return res;
}

std::vector<XYVector> makeVelocimeterLocations()
{
  const float targetR = 1;
  std::normal_distribution<float> rNoise(0.0f, targetR * 0.15f);

  std::vector<XYVector> res(N_VELOCIMETERS);
  for (int i = 0; i < N_VELOCIMETERS; ++i)
  {
    const float r = 1 + rNoise(randomGenerator);
    const float t = i * 2 * M_PI/ N_VELOCIMETERS;
    res[i] = polarToCart(r, t);
  }

  return res;
}

int main()
{
  std::cout << "makedata\n";
  auto vectorData = makeVectorData();
  writeVector<VectorDataItem>(vectorData, VECTOR_DATA_PATH);

  auto velocimeterLocations = makeVelocimeterLocations();
  writeVector<XYVector>(velocimeterLocations, VELOCIMETER_LOCATIONS_PATH);
}