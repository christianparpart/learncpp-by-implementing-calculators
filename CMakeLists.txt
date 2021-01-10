cmake_minimum_required(VERSION 3.6)
project(learncpp-by-calculators VERSION "0.0.0" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT MSVC)
  add_definitions(-Wall)
  add_definitions(-pedantic)
endif()

add_executable(level_1 level_1.cpp)
add_executable(level_2 level_2.cpp)
add_executable(level_3 level_3.cpp)
add_executable(level_4 level_4.cpp)
add_subdirectory(./level_5)
