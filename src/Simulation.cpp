#include <Colormap.hpp>
#include <Simulation.hpp>
#include <atomic>
#include <config.hpp>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

#include <cmath> // switch to kmath

Simulation::Simulation() {}

void Simulation::advance(FlowVector v)
{
  updateData(v);
  createNewParcel();
}

void Simulation::writeFrame(uint32_t *buffer, const int numThreads) const
{
  static double maxZ = 0.01 / config::SCALING_FACTOR;

  static const int xRatio = config::WINDOW_RESOLUTION / config::SIMULATION_RESOLUTION;
  static const int yRatio = config::WINDOW_RESOLUTION / config::SIMULATION_RESOLUTION;

  static const int xBorder = (config::WINDOW_RESOLUTION % config::SIMULATION_RESOLUTION) / 2;
  static const int yBorder = (config::WINDOW_RESOLUTION % config::SIMULATION_RESOLUTION) / 2;

  std::atomic<int> nextIndex(0);

  auto threadWork = [&]()
  {
    while (true)
    {
      int i = nextIndex.fetch_add(1);
      if (i >= config::SIMULATION_RESOLUTION * config::SIMULATION_RESOLUTION)
      {
        break;
      }

      const int x = i % config::SIMULATION_RESOLUTION;
      const int y = i / config::SIMULATION_RESOLUTION;

      const auto density = getDensityAtXY(x, y);
      const auto color = Colormap::getColor(density, maxZ);

      for (int j = 0; j < yRatio; ++j)
      {
        const int index = xBorder + x * xRatio + (y * yRatio + j + yBorder) * config::WINDOW_RESOLUTION;
        std::fill_n(buffer + index, xRatio, color);
      }
    }
  };

  std::vector<std::thread> threads;
  for (int i = 0; i < numThreads; ++i)
  {
    threads.emplace_back(std::move(threadWork));
  }

  for (auto &t : threads)
  {
    t.join();
  }
}

void Simulation::createNewParcel()
{
  if (config::MAX_NUM_ACTIVE_PARCELS > 0 && data.size() == config::MAX_NUM_ACTIVE_PARCELS - 1)
  {
    data.erase(data.begin());
  }
  data.emplace_back(data.size());
}

void Simulation::updateData(FlowVector v)
{
  for (auto &parcel : data)
  {
    parcel.x += v.x * config::SIMULATION_TIMESTEP * config::VELOCITY_COEFFICIENT * config::SCALING_FACTOR;
    parcel.y += v.y * config::SIMULATION_TIMESTEP * config::VELOCITY_COEFFICIENT * config::SCALING_FACTOR;
    parcel.sx *= config::DISPERSION_COEFFICIENT;
    parcel.sy *= config::DISPERSION_COEFFICIENT;
  }
};

double Simulation::getDensityAtXY(const double x, const double y) const
{
  double density = 0;
  for (const auto &parcel : data)
  {
    const double dx = x - parcel.x;
    const double dy = y - parcel.y;
    const double sx2 = parcel.sx * parcel.sx;
    const double sy2 = parcel.sy * parcel.sy;

    const double expression = config::SIMULATION_TIMESTEP / (2 * M_PI * parcel.sx * parcel.sy);
    const double exponent = -(dx * dx / (2 * sx2) + dy * dy / (2 * sy2));

    density += expression * std::exp(exponent);
  }

  return density;
}