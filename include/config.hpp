#pragma once

namespace config
{
inline int WINDOW_WIDTH = 1000;
inline int WINDOW_HEIGHT = 1000;

// should be divisor of WINDOW parameters
inline int SIMULATION_WIDTH = 100;
inline int SIMULATION_HEIGHT = 100;

inline double TARGET_FPS = 60.0;

// simulation parameters
inline double PARCEL_INITIAL_SIGMA = 0.1;
inline double SIMULATION_TIMESTEP = 0.09;
inline double DISPERSION_COEFFICIENT = 1.03;
inline double MAX_NUM_ACTIVE_PARCELS = 100; // -1 for no limit
} // namespace config