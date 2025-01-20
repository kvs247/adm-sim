#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "common/config.hpp"
#include "distribution.hpp"

std::vector<float> Distribution::calculateDistributions() const
{
  const auto &config = Config::getInstance().getConfig();

  std::vector<float> res(config.animationDimension * config.animationDimension * config.nReadings);

  std::atomic<size_t> nextIndex(0);

  const auto locations = makeLocations();

  auto processChunk = [&]()
  {
    while (true)
    {
      size_t t = nextIndex.fetch_add(1);
      if (t >= config.nReadings)
      {
        break;
      }

      for (size_t i = 0; i < config.animationDimension; ++i)
      {
        for (size_t j = 0; j < config.animationDimension; ++j)
        {
          const auto x = locations[i];
          const auto y = locations[j];
          const auto pointDensity = calculateDensityAtXYT(x, y, t);

          size_t updateIndex = t * config.animationDimension * config.animationDimension +
                               i * config.animationDimension + j;
          res[updateIndex] = pointDensity;
        }
      }
    }
  };

  const size_t maxNumThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;
  for (size_t i = 0; i < maxNumThreads; ++i)
  {
    threads.emplace_back(std::move(processChunk));
  }

  for (auto &t : threads)
  {
    t.join();
  }

  return res;
};

float Distribution::calculateDensityAtXYT(const float x, const float y, const float t) const
{
  const auto &config = Config::getInstance().getConfig();

  float density = 0;
  for (const auto &parcelLife : simulationData)
  {
    const auto initialParcel = parcelLife.front();
    if (initialParcel.t > t)
    {
      continue;
    }

    const auto parcelAtT = parcelLife[t - initialParcel.t];

    const float dx = x - parcelAtT.x;
    const float dy = y - parcelAtT.y;
    const float sx2 = parcelAtT.sx * parcelAtT.sx;
    const float sy2 = parcelAtT.sy * parcelAtT.sy;

    const float expression = config.timestep / (2 * M_PI * parcelAtT.sx * parcelAtT.sy);
    const float exponent = -(dx * dx / (2 * sx2) + dy * dy / (2 * sy2));

    density += expression * std::exp(exponent);
  }

  return density;
};

std::vector<float> Distribution::makeLocations() const
{
  const auto &config = Config::getInstance().getConfig();

  std::vector<float> locations;
  const float border = config.animationWidth / 2;
  const float step = config.animationWidth / config.animationDimension;
  for (float i = -border; i < border; i += step)
  {
    locations.push_back(i);
  }

  return locations;
};