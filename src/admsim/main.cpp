#include <iostream>

#include "common/config.hpp"
#include "common/io.hpp"
#include "common/types.hpp"

int main()
{
  std::cout << "admsim\n";

  const auto data = readVector<VelocimetersItem>(VECTOR_DATA_PATH);
  std::cout << "datal: " << data.size() << "\n";
  for (auto &item : data)
  {
    std::cout << "{\n";
    for (auto j = 0; j < N_VELOCIMETERS; ++j)
    {
      std::cout << "\t" << j << ": {" << item[j].x << ", " << item[j].y << "},\n";
    }
    std::cout << "},\n";
  }
  std::cout << "\n";
}