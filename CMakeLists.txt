cmake_minimum_required(VERSION 3.10)
if(NOT NO_HERMIT_CORE)
    include(cmake/HermitCore-Application.cmake)
endif()

project("Monte Carlo Integration")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(lib)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -mtune=native)
endif()

link_libraries(mc_integration)

add_executable(time_measure src/time_measure.cpp)
add_executable(pi src/pi.cpp)
add_executable(multi_dim src/multi_dim.cpp)
target_link_libraries(pi -lm)

