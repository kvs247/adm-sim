cmake_minimum_required(VERSION 3.28)
project(AdvectionDiffusionSimulation)

include(FetchContent)

FetchContent_Declare(
  kmath
  GIT_REPOSITORY https://github.com/kvs247/kmath.git
  GIT_TAG main
)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(kmath json googletest)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/out)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(common INTERFACE nlohmann_json::nlohmann_json)

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
