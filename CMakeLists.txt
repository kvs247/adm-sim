cmake_minimum_required(VERSION 3.28)
project(admsim)

add_compile_options(-Wall -Wextra)

find_package(X11 REQUIRED)

include(FetchContent)

FetchContent_Declare(
  kmath
  GIT_REPOSITORY https://github.com/kvs247/kmath.git
  GIT_TAG main
)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(kmath googletest)

add_executable(admsim
  src/main.cpp
)
