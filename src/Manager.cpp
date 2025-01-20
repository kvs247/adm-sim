#include <Manager.hpp>
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <chrono>
#include <config.hpp>
#include <iostream>
#include <thread>

Manager::Manager() : renderer(), running(false), state{0, 0} {}

void Manager::start()
{
  running = true;
  mainLoop();
};

void Manager::stop() { running = false; };

void Manager::mainLoop()
{
  const std::chrono::duration<double> TARGET_FRAME_DURATION(1.0 / config::TARGET_FPS);
  auto lastFrameTime = std::chrono::steady_clock::now();

  int frameNumber = 0;
  while (running)
  {
    const auto frameStart = std::chrono::steady_clock::now();

    FrameMetrics frame;
    frame.frameNumber = frameNumber++;

    processEvents();
    updateState();
    updateDisplay();

    const auto frameEnd = std::chrono::steady_clock::now();
    const auto frameDuration = frameEnd - frameStart;

    frame.totalTime = static_cast<double>(frameDuration.count() / 1e9);
    frame.framesPerSecond = 1.0 / frame.totalTime;

    if (frameNumber % (int)config::TARGET_FPS == 0)
    {
      std::cout << frame.toString() << "\n";
    }

    if (frameDuration < TARGET_FRAME_DURATION)
    {
      std::this_thread::sleep_for(TARGET_FRAME_DURATION - frameDuration);
    }
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
      const auto keysym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
      if (keysym == XK_q || keysym == XK_x || keysym == XK_Escape)
      {
        stop();
      }
    }
  }
};

void Manager::updateState() {};

void Manager::updateDisplay()
{
  auto buffer = renderer.getBackBuffer();
  std::fill_n(buffer, config::WINDOW_WIDTH * config::WINDOW_HEIGHT, 0xffffff);

  const int radius = 10;
  for (int y = -radius; y <= radius; ++y)
  {
    for (int x = -radius; x <= radius; ++x)
    {
      if (x * x + y * y > radius * radius)
      {
        continue;
      }

      const int bufferX = state.cursorX + x;
      const int bufferY = state.cursorY + y;
      if (bufferX < 0 || bufferX >= config::WINDOW_WIDTH || bufferY < 0 || bufferY >= config::WINDOW_WIDTH)
      {
        continue;
      }

      buffer[bufferY * config::WINDOW_WIDTH + bufferX] = 0x000000;
    }
  }

  renderer.render();
};