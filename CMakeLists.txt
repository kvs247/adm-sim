cmake_minimum_required(VERSION 3.28)
project(AdvectionDiffusionSimulation)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)
FetchContent_MakeAvailable(googletest)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_library(common STATIC
  src/common/config.cpp
)
target_include_directories(common 
  PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(common 
  PUBLIC 
    nlohmann_json::nlohmann_json
)

add_subdirectory(src/admsim)
add_subdirectory(src/makedata)

enable_testing()

add_custom_target(run_all
  COMMAND makedata
  COMMAND admsim
  COMMAND python3 ${CMAKE_SOURCE_DIR}/src/makeanimation/make_animation.py ${CMAKE_BINARY_DIR}/data/distributionData
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  DEPENDS makedata admsim
)