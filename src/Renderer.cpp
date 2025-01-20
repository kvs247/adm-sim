#include <Renderer.hpp>
#include <X11/Xlib.h>
#include <config.hpp>
#include <cstdint>
#include <memory>
#include <stdexcept>

Renderer::Renderer()
{
  display = XOpenDisplay(nullptr);
  if (!display)
  {
    throw std::runtime_error("unable to open X display");
  }

  screen = DefaultScreen(display);
  root = RootWindow(display, screen);
  visual = DefaultVisual(display, screen);

  XSetWindowAttributes windowAttributes = {};
  windowAttributes.background_pixel = WhitePixel(display, screen);
  windowAttributes.border_pixel = BlackPixel(display, screen);
  windowAttributes.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
  windowAttributes.override_redirect = true;
  window = XCreateWindow(
      display,
      root,
      0,
      0,
      config::WINDOW_WIDTH,
      config::WINDOW_HEIGHT,
      0,
      DefaultDepth(display, screen),
      InputOutput,
      visual,
      CWBackPixel | CWEventMask | CWBorderPixel,
      &windowAttributes);

  // remove header decoration
  Atom wm_delete = XInternAtom(display, "WM_DELETE_WINDOW", true);
  XSetWMProtocols(display, window, &wm_delete, 1);
  Atom wm_hints = XInternAtom(display, "_MOTIF_WM_HINTS", true);
  if (wm_hints != None)
  {
    struct
    {
      unsigned long flags;
      unsigned long functions;
      unsigned long decorations;
      long input_mode;
      unsigned long status;
    } hints = {2, 0, 0, 0, 0};
    XChangeProperty(display, window, wm_hints, wm_hints, 32, PropModeReplace, (unsigned char *)&hints, 5);
  }

  XMapWindow(display, window);

  XStoreName(display, window, "Advection-Difussion Model Simulation");

  initializeGC();
  initializeBuffers();
}

Renderer::~Renderer()
{
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
}

void Renderer::render()
{
  image->data = reinterpret_cast<char *>(backBuffer.get());
  XPutImage(display, window, gc, image, 0, 0, 0, 0, config::WINDOW_WIDTH, config::WINDOW_HEIGHT);
  frontBuffer.swap(backBuffer);
}

void Renderer::initializeGC()
{
  XGCValues xgcv;

  xgcv.line_style = LineSolid;
  xgcv.cap_style = CapButt;
  xgcv.join_style = JoinMiter;
  xgcv.fill_style = FillSolid;
  xgcv.foreground = BlackPixel(display, screen);
  xgcv.background = WhitePixel(display, screen);

  unsigned long valueMask = GCForeground | GCBackground | GCFillStyle | GCLineStyle | GCLineWidth | GCLineWidth |
                            GCCapStyle | GCJoinStyle;

  gc = XCreateGC(display, root, valueMask, &xgcv);
}

void Renderer::initializeBuffers()
{
  const int bufferSize = config::WINDOW_WIDTH * config::WINDOW_HEIGHT;
  frontBuffer = std::make_unique<uint32_t[]>(bufferSize);
  backBuffer = std::make_unique<uint32_t[]>(bufferSize);

  std::fill_n(frontBuffer.get(), config::WINDOW_HEIGHT * config::WINDOW_WIDTH, 0x000000);
  std::fill_n(backBuffer.get(), config::WINDOW_HEIGHT * config::WINDOW_WIDTH, 0x000000);

  image = XCreateImage(
      display,
      visual,
      DefaultDepth(display, screen),
      ZPixmap,
      0,
      reinterpret_cast<char *>(backBuffer.get()),
      config::WINDOW_WIDTH,
      config::WINDOW_HEIGHT,
      32,
      config::WINDOW_WIDTH * sizeof(uint32_t));
}