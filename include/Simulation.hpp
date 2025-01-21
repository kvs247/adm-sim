#pragma once

#include <config.hpp>
#include <cstdint>
#include <vector>

class Simulation
{
public:
  Simulation();

  struct Parcel
  {
    int id;
    double x;
    double y;
    double sx;
    double sy;

    Parcel(
        int _id,
        double _x = config::SIMULATION_WIDTH / 2,
        double _y = config::SIMULATION_HEIGHT / 2,
        double _sx = config::PARCEL_INITIAL_SIGMA,
        double _sy = config::PARCEL_INITIAL_SIGMA)
        : id(_id), x(_x), y(_y), sx(_sx), sy(_sy)
    {
    }
  };

  struct FlowVector
  {
    double x;
    double y;
  };

  const std::vector<Parcel> &getData() { return data; }

  void advance(FlowVector v);
  void writeFrame(uint32_t *bufffer) const;

private:
  std::vector<Parcel> data = {};

  void createNewParcel();
  void updateData(FlowVector v);
  double getDensityAtXY(const double x, const double y) const;
};