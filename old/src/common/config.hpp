#pragma once

#include <fstream>
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

  static Config &getInstance()
  {
    const std::string configPath = "../config.json";
    static Config instance(configPath);
    return instance;
  };
  const Settings &getConfig() const { return settings; }

private:
  Config(const std::string &filepath)
  {
    std::ifstream f(filepath);
    if (!f.is_open())
    {
      throw std::runtime_error("Error opening file: " + filepath);
    }

    const auto j = nlohmann::json::parse(f);

    settings.nVelocimeters = j["N_VELOCIMETERS"];
    settings.nReadings = j["N_READINGS"];
    settings.animationDimension = j["ANIMATION_DIMENSION"];
    settings.animationWidth = j["ANIMATION_WIDTH"];
    settings.timestep = j["TIMESTEP"];
    settings.parcelXOrigin = j["PARCEL_X_ORIGIN"];
    settings.parcelYOrigin = j["PARCEL_Y_ORIGIN"];
    settings.initialParcelDispersion = j["INITIAL_PARCEL_DISPERSION"];
    settings.dispersionGrowthCoefficient = j["DISPERSION_GROWTH_COEFFICIENT"];
    settings.vectorDataPath = j["VECTOR_DATA_PATH"];
    settings.velocimeterLocationsPath = j["VELOCIMETER_LOCATIONS_PATH"];
  }
  Config(const Config &) = delete;
  Config &operator=(const Config &) = delete;

  Settings settings;
};