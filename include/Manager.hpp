#pragma once

#include <sstream>
#include <string>

#include "Renderer.hpp"

class Manager
{
public:
  Manager();

  void start();
  void stop();

private:
  void mainLoop();
  void processEvents();
  void updateState();
  void updateDisplay();

  Renderer renderer;

  bool running;
  struct State
  {
    int cursorX;
    int cursorY;
  } state;

  struct FrameMetrics
  {
    int frameNumber;
    double framesPerSecond;
    double totalTime;
    // double simulationTime;
    // double colorProcessingTime;
    // double renderTime

    std::string toString()
    {
      std::stringstream ss;
      ss << "i: " << frameNumber << ", fps: " << framesPerSecond << ", t: " << totalTime;
      return ss.str();
    }
  };
};