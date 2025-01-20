#include <fstream>
#include <iostream>
#include <kmath/Trig.hpp>
#include <map>
#include <random>
#include <tuple>
#include <vector>

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/types.hpp"

const float targetDirectionDeg = 135;
const float directionSpreadDeg = 5.0f;
const float targetSpeed = 6;
const float speedSpreadPercent = 0.1f;

std::mt19937 randomGenerator(std::random_device{}());

std::vector<VectorDataItem> makeVectorData()
{
  const auto &config = Config::getInstance().getConfig();

  // could also do uniform_real_distribution
  std::normal_distribution<float> rNoise(0.0f, targetSpeed * speedSpreadPercent);
  std::normal_distribution<float> tNoise(0.0f, kmath::degToRad(directionSpreadDeg));

  std::uniform_real_distribution<float> xNoise(-10, 10);

  float dir = targetDirectionDeg;

  std::vector<VectorDataItem> res(config.nReadings);
  for (int i = 0; i < config.nReadings; ++i)
  {

    dir += xNoise(randomGenerator);

    VectorDataItem resItem;
    for (int j = 0; j < config.nVelocimeters; ++j)
    {
      const float r = targetSpeed + rNoise(randomGenerator);
      const float t = kmath::degToRad(dir) + tNoise(randomGenerator);
      std::tie(resItem[j].x, resItem[j].y) = kmath::polarToCart(r, t);
    }
    res[i] = resItem;
  }

  return res;
}

std::vector<XYVector> makeVelocimeterLocations()
{
  const auto &config = Config::getInstance().getConfig();

  const float targetR = 1;
  std::normal_distribution<float> rNoise(0.0f, targetR * 0.15f);

  std::vector<XYVector> res(config.nVelocimeters);
  for (int i = 0; i < config.nVelocimeters; ++i)
  {
    const float r = 1 + rNoise(randomGenerator);
    const float t = i * 2 * M_PI / config.nVelocimeters;
    std::tie(res[i].x, res[i].y) = kmath::polarToCart(r, t);
  }

  return res;
}

int main()
{
  const auto &config = Config::getInstance().getConfig();

  std::cout << "executing makedata...\n";
  auto vectorData = makeVectorData();
  writeVector<VectorDataItem>(vectorData, config.vectorDataPath);
  logDataWritten<VectorDataItem>(vectorData, config.vectorDataPath);

  auto velocimeterLocations = makeVelocimeterLocations();
  writeVector<XYVector>(velocimeterLocations, config.velocimeterLocationsPath);
  logDataWritten<XYVector>(velocimeterLocations, config.velocimeterLocationsPath);
}