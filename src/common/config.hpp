#pragma once

#include <nlohmann/json.hpp>
#include <string>

class Config
{
public:
  struct Settings
  {
    size_t nVelocimeters;
    int nReadings;
    int animationDimension;
    float animationWidth;
    float timestep;
    float parcelXOrigin;
    float parcelYOrigin;
    float initialParcelDispersion;
    float dispersionGrowthCoefficient;
    std::string vectorDataPath;
    std::string velocimeterLocationsPath;
  };

  static Config &getInstance();
  const Settings &getConfig() const { return settings; }

private:
  Config(const std::string &filepath);
  Settings settings;

  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;
};