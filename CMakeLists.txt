cmake_minimum_required(VERSION 3.10)
include(cmake/HermitCore-Application.cmake)

# set the project name
project("Monte Carlo Integration")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(mc_integration src/mc_integration.cpp)
