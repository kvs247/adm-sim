cmake_minimum_required(VERSION 3.28)
project(AdvectionDiffusionSimulation)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out)

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

add_custom_target(run_all
  COMMAND makedata
  COMMAND admsim
  COMMAND python3 ${CMAKE_SOURCE_DIR}/src/makeanimation/make_animation.py ${CMAKE_BINARY_DIR}/data/distributionData
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS makedata admsim
)