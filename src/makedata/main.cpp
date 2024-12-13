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

std::vector<VelocimetersItem> makeVectorData()
{
  std::mt19937 randomGenerator(std::random_device{}());

  // could also do uniform_real_distribution
  std::normal_distribution<float> rNoise(0.0f, targetSpeed * speedSpreadPercent);
  std::normal_distribution<float> tNoise(0.0f, degToRad(directionSpreadDeg));

  std::vector<VelocimetersItem> res(N_READINGS);
  for (int i = 0; i < N_READINGS; ++i)
  {
    VelocimetersItem resItem;
    for (int j = 0; j < N_VELOCIMETERS; ++j)
    {
      float r = targetSpeed + rNoise(randomGenerator);
      float t = degToRad(targetDirectionDeg) + tNoise(randomGenerator);
      resItem[j] = polarToCart(r, t);
    }
    res[i] = resItem;
  }

  return res;
}

int main()
{
  std::cout << "makedata\n";
  auto vectorData = makeVectorData();
  writeVector<VelocimetersItem>(vectorData, VECTOR_DATA_PATH);
}