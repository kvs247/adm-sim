#pragma once

#include <cstdint>
#include <thread>

namespace config
{
inline unsigned int MAX_THREADS = std::thread::hardware_concurrency();

inline int WINDOW_WIDTH = 1080;
inline int WINDOW_HEIGHT = 1080;

// should be divisor of WINDOW parameters
inline int SIMULATION_WIDTH = 250;
inline int SIMULATION_HEIGHT = 250;

inline double TARGET_FPS = 60.0;

// simulation parameters
inline double PARCEL_INITIAL_SIGMA = 2.0;
inline double SIMULATION_TIMESTEP = 0.09;
inline double DISPERSION_COEFFICIENT = 1.01;
inline double MAX_NUM_ACTIVE_PARCELS = 100; // -1 for no limit
inline double WIND_NOISE = 0.0;

// colors
inline uint32_t DIM_GREY = 0x696969;
} // namespace config