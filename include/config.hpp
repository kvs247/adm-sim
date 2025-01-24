#pragma once

#include <cstdint>
#include <thread>

namespace config
{
inline unsigned int MAX_THREADS = std::thread::hardware_concurrency();

inline int WINDOW_RESOLUTION = 1005;

inline int SIMULATION_RESOLUTION = 250;

inline int SCALING_FACTOR = SIMULATION_RESOLUTION / 50;

inline double TARGET_FPS = 60.0;

// simulation parameters
inline double PARCEL_INITIAL_SIGMA = 1.0 * SCALING_FACTOR;
inline double SIMULATION_TIMESTEP = 0.09;
inline double DISPERSION_COEFFICIENT = 1.05;
inline double MAX_NUM_ACTIVE_PARCELS = 100; // -1 for no limit
inline double VELOCITY_COEFFICIENT = 10.0;
inline double VELOCITY_MULT_INCREMENT = 0.1;

// colors
inline uint32_t DIM_GREY = 0x696969;
} // namespace config