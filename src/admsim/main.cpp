#include <iostream>

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/types.hpp"

int main()
{
  std::cout << "admsim\n";

  const auto vectorData = readVector<VectorDataItem>(VECTOR_DATA_PATH);
  for (auto &item : vectorData)
  {
    std::cout << "{\n";
    for (auto j = 0; j < N_VELOCIMETERS; ++j)
    {
      std::cout << "\t" << j << ": {" << item[j].x << ", " << item[j].y << "},\n";
    }
    std::cout << "},\n";
  }
  std::cout << "\n";

  std::cout << "\n\n";
  const auto velocimetersData = readVector<XYVector>(VELOCIMETER_LOCATIONS_PATH);
  for (auto &item : velocimetersData)
  {
    std::cout << "{" << item.x << ", " << item.y << "}\n";
  }
}