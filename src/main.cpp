#include <Manager.hpp>
#include <iostream>

int main()
{
  try
  {
    Manager manager;
    manager.start();

    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}