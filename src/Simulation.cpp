#include <Simulation.hpp>
#include <config.hpp>
#include <cstdint>

#include <cmath> // switch to kmath
#include <iostream>

Simulation::Simulation() {}

void Simulation::advance(FlowVector v)
{
  updateData(v);
  createNewParcel();
}

uint32_t mapToHex(double value, double maxValue)
{
  // Check if value is within valid range
  if (value < 0 || value > maxValue)
  {
    return 0xffffff;
  }

  // Handle edge cases
  if (value == 0)
    return 0;
  if (value == maxValue)
    return std::numeric_limits<uint32_t>::max();

  // Perform linear mapping
  double normalized = value / maxValue;
  return static_cast<uint32_t>(normalized * std::numeric_limits<uint32_t>::max());
}

void Simulation::writeFrame(uint32_t *buffer) const
{
  static double maxZ = 0.1;

  const int xRatio = config::WINDOW_WIDTH / config::SIMULATION_WIDTH;
  const int yRatio = config::WINDOW_HEIGHT / config::SIMULATION_HEIGHT;

  for (int x = 0; x < config::SIMULATION_WIDTH; ++x)
  {
    for (int y = 0; y < config::SIMULATION_HEIGHT; ++y)
    {
      const auto z = getDensityAtXY(x, y);
      maxZ = std::max(z, maxZ);
      const uint32_t c = mapToHex(z / maxZ, 1);
      // const uint32_t c = z > 0.1 ? 0xffffff : 0x000000;

      for (int j = 0; j < yRatio; ++j)
      {
        const int index = x * xRatio + (y * yRatio + j) * config::WINDOW_WIDTH;
        std::fill_n(buffer + index, xRatio, c);
      }
    }
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
    parcel.x += v.x * config::SIMULATION_TIMESTEP * 10;
    parcel.y += v.y * config::SIMULATION_TIMESTEP * 10;
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

  return 100000 * density;
}