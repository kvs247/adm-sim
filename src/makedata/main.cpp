#include <fstream>
#include <iostream>
#include <math.h>
#include <random>
#include <vector>

#include "common/io.hpp"
#include "common/math.hpp"
#include "common/types.hpp"

const float targetDirectionDeg = 135;
const float directionSpreadDeg = 5.0f;
const float targetSpeed = 6;
const float speedSpreadPercent = 0.1f;
const int nMonitors = 4;
const int dataSize = 100;

std::vector<XYVector> makeVectorData()
{
  std::mt19937 randomGenerator(std::random_device{}());

  // could also do uniform_real_distribution
  std::normal_distribution<float> rNoise(0.0f, targetSpeed * speedSpreadPercent);
  std::normal_distribution<float> tNoise(0.0f, degToRad(directionSpreadDeg));

  std::vector<XYVector> res(dataSize);
  for (int i = 0; i < dataSize; ++i)
  {
    float r = targetSpeed + rNoise(randomGenerator);
    float t = degToRad(targetDirectionDeg) + tNoise(randomGenerator);
    res[i] = polarToCart(r, t);
  }

  return res;
}

int main()
{
  std::vector<XYVector> vectorData = makeVectorData();
  const std::string fpath = "vectorData";
  writeVector<XYVector>(vectorData, fpath);

  const auto data = readVector<XYVector>(fpath);
  for (auto &i : data)
  {
    std::cout << "{" << i.x << ", " << i.y << "}, ";
  }
  std::cout << "\n";

  std::cout << "makedata\n";
}