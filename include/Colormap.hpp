#include <array>
#include <cmath>
#include <cstdint>
#include <vector>

class Colormap
{
private:
  struct RGB
  {
    double r, g, b;
  };

  static constexpr std::array<RGB, 6> controlPoints = {{
      {0.001462, 0.000466, 0.013866}, // Nearly black
      {0.173816, 0.021872, 0.255691}, // Deep purple
      {0.454414, 0.063536, 0.413792}, // Purple
      {0.731904, 0.170464, 0.418399}, // Pink
      {0.927623, 0.362529, 0.313350}, // Orange
      {0.988362, 0.644924, 0.144154}  // Yellow
  }};

public:
  static uint32_t getColor(double value, double maxValue)
  {
    const double x = (value > maxValue) ? 1.0 : value / maxValue;

    const double position = x * (controlPoints.size() - 1);
    const size_t index = x * (controlPoints.size() - 1);

    const double interpDist = position - index;

    if (interpDist == 0)
    {
      return rgbToHex(controlPoints[index]);
    }

    const auto p1 = controlPoints[index];
    const auto p2 = controlPoints[index + 1];
    const double r = linearInterp(p1.r, p2.r, interpDist);
    const double g = linearInterp(p1.g, p2.g, interpDist);
    const double b = linearInterp(p1.b, p2.b, interpDist);

    return rgbToHex({r, g, b});
  }

private:
  static double linearInterp(const double a, const double b, const double x) { return a + x * (b - a); }

  static uint32_t rgbToHex(const RGB &rgb)
  {
    uint8_t r = static_cast<uint8_t>(std::round(rgb.r * 255));
    uint8_t g = static_cast<uint8_t>(std::round(rgb.g * 255));
    uint8_t b = static_cast<uint8_t>(std::round(rgb.b * 255));

    return (static_cast<uint32_t>(r) << 16) | (static_cast<uint32_t>(g) << 8) | (static_cast<uint32_t>(b));
  }
};