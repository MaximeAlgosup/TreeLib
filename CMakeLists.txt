# How to use: (Windows)
#   add googletest to the directory root
#   cmake -S . -B Build -G "MinGW Makefiles"
#   cd Build
#   mingw32-make

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

project(
  tests
  VERSION 1.0
  LANGUAGES CXX C
)

find_package(Threads REQUIRED)

add_executable(tests
  treeLib.c
  tests.cc
  googletest/googletest/src/gtest-all.cc
)

target_include_directories(tests
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)

target_link_libraries(tests
  PRIVATE
    Threads::Threads
    # Add other libraries if needed
)

target_compile_options(tests
  PRIVATE
    -Wall -Wextra -pedantic -g -O2
)

set_target_properties(tests
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
