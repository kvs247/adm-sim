cmake_minimum_required(VERSION 3.28)
project(admsim)

add_compile_options(-Wall -Wextra)

find_package(X11 REQUIRED)

add_executable(admsim
  src/main.cpp
  src/Manager.cpp
  src/Renderer.cpp
  src/Simulation.cpp
)

target_include_directories(admsim
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(admsim PRIVATE X11)
