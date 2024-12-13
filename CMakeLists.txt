cmake_minimum_required(VERSION 3.28)
project(AdvectionDiffusionSimulation)

enable_testing()

# Google test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(src/admsim)

add_subdirectory(src/makedata)