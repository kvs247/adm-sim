#pragma once

#include <X11/Xlib.h>
#include <cstdint>
#include <memory>

class Renderer
{
public:
  Renderer();
  ~Renderer();

  Display *getDisplay() { return display; }
  uint32_t *getBackBuffer() { return backBuffer.get(); }

  void render();

private:
  Display *display;
  Window root;
  Window window;
  Visual *visual;
  GC gc;
  int screen;
  XImage *image;
  std::unique_ptr<uint32_t[]> frontBuffer;
  std::unique_ptr<uint32_t[]> backBuffer;

  void initializeGC();
  void initializeBuffers();
};