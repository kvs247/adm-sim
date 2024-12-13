cmake_minimum_required(VERSION 3.28)
project(AdvectionDiffusionSimulation)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src/admsim)

add_subdirectory(src/makedata)