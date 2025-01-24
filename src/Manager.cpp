#include <Manager.hpp>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <chrono>
#include <config.hpp>
#include <iostream>
#include <thread>

#include <cmath> // use kmath

namespace
{
struct FrameMetrics
{
  int frameNumber;
  int numThreads;
  double framesPerSecond;
  double totalTime;
  double eventProcessingTime;
  double simulationTime;
  double frameGenerationTime;
  double renderTime;

  std::string toString()
  {
    char buffer[100];
    snprintf(
        buffer,
        sizeof(buffer),
        "i: %4d, fps: %.0f, nCPU: %d, t: %.0fms [events: %.1f, sim: %.1f, frame: %.0f, render; %0.1f]",
        frameNumber,
        framesPerSecond,
        numThreads,
        totalTime * 1000,
        eventProcessingTime * 1000,
        simulationTime * 1000,
        frameGenerationTime * 1000,
        renderTime * 100);
    return std::string(buffer);
  }
};
} // anonymous namespace

Manager::Manager() : renderer(), simulation(), state{false, 1, 1.0, 0, 0} {}

void Manager::start()
{
  state.running = true;
  mainLoop();
};

void Manager::stop() { state.running = false; };

void Manager::mainLoop()
{
  const std::chrono::duration<double> TARGET_FRAME_DURATION(1.0 / config::TARGET_FPS);
  auto lastFrameTime = now();

  int frameNumber = 0;
  while (state.running)
  {
    const auto frameStart = now();

    FrameMetrics frame;
    frame.frameNumber = frameNumber;
    frame.numThreads = state.numThreads;

    auto eventsStart = now();
    processEvents();
    frame.eventProcessingTime = getDurationSeconds(eventsStart, now());

    auto simStart = now();
    advanceSimulation();
    frame.simulationTime = getDurationSeconds(simStart, now());

    auto frameGenerationStart = now();
    generateFrame();
    frame.frameGenerationTime = getDurationSeconds(frameGenerationStart, now());

    auto renderStart = now();
    updateDisplay();
    frame.renderTime = getDurationSeconds(renderStart, now());

    const auto frameEnd = now();
    const auto frameDuration = frameEnd - frameStart;

    frame.totalTime = getDurationSeconds(frameStart, frameEnd);
    frame.framesPerSecond = 1.0 / frame.totalTime;

    if ((frameNumber % (int)config::TARGET_FPS) == 0)
    {
      std::cout << frame.toString() << "\n";
    }

    if (frameDuration < TARGET_FRAME_DURATION)
    {
      if (state.numThreads > 1)
      {
        --state.numThreads;
      }
      std::this_thread::sleep_for(TARGET_FRAME_DURATION - frameDuration);
    }
    else
    {
      if (state.numThreads < config::MAX_THREADS)
      {
        ++state.numThreads;
      }
    }

    ++frameNumber;
  }
};

void Manager::processEvents()
{
  XEvent event;
  auto display = renderer.getDisplay();

  while (XPending(display))
  {
    XNextEvent(display, &event);
    switch (event.type)
    {
    case MotionNotify:
      state.cursorX = event.xmotion.x;
      state.cursorY = event.xmotion.y;
      break;
    case KeyPress:
    {
      const auto keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
      if (keysym == XK_q || keysym == XK_x || keysym == XK_Escape)
      {
        stop();
      }
      break;
    }
    case ButtonPress:
      if (event.xbutton.button == Button4)
      {
        state.velocityMult += config::VELOCITY_MULT_INCREMENT;
      }
      if (event.xbutton.button == Button5)
      {
        state.velocityMult = std::max(0.0, state.velocityMult - config::VELOCITY_MULT_INCREMENT);
      }
      break;
    }
  }
};

void Manager::advanceSimulation()
{
  double flowX = state.cursorX - config::WINDOW_RESOLUTION / 2;
  double flowY = state.cursorY - config::WINDOW_RESOLUTION / 2;

  const double n = std::hypot(flowX, flowY);
  simulation.advance({state.velocityMult * flowX / n, state.velocityMult * flowY / n});
};

void Manager::generateFrame() { simulation.writeFrame(renderer.getBackBuffer(), state.numThreads); };

void Manager::updateDisplay() { renderer.render(); };

Manager::TimePoint Manager::now() { return std::chrono::steady_clock::now(); }

double Manager::getDurationSeconds(const Manager::TimePoint start, const Manager::TimePoint end)
{
  if (start > end)
  {
    throw std::runtime_error("start TimePoint cannot be after end TimePoint");
  }

  return std::chrono::duration<double>(end - start).count();
}