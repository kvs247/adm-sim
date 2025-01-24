#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#include "Renderer.hpp"
#include "Simulation.hpp"

class Manager
{
public:
  Manager();

  void start();
  void stop();

private:
  Renderer renderer;
  Simulation simulation;
  struct State
  {
    bool running;
    int numThreads;
    double velocityMult;
    int cursorX;
    int cursorY;
  } state;

  void mainLoop();
  void processEvents();
  void advanceSimulation();
  void generateFrame();
  void updateDisplay();

  using TimePoint = std::chrono::steady_clock::time_point;
  static TimePoint now();
  static double getDurationSeconds(const TimePoint start, const TimePoint end);
};