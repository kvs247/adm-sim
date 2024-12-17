#include "config.hpp"
#include <fstream>
#include <stdexcept>

Config &Config::getInstance()
{
  const std::string configPath = "../config.json";
  static Config instance(configPath);
  return instance;
}

Config::Config(const std::string &filepath)
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