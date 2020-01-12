cmake_minimum_required(VERSION 3.10)
if(NOT NO_HERMIT_CORE)
    include(cmake/HermitCore-Application.cmake)
endif()

# set the project name
project("Monte Carlo Integration")

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add the executable
add_executable(mc_integration src/mc_integration.cpp)
target_compile_options(mc_integration PRIVATE -fopenmp)
target_link_libraries(mc_integration -fopenmp)

if(MSVC)
    target_compile_options(mc_integration PRIVATE /W4 /WX)
else()
    target_compile_options(mc_integration PRIVATE -Wall -Wextra -pedantic)
endif()
