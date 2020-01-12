cmake_minimum_required(VERSION 3.10)
add_library(mc_integration INTERFACE)

target_include_directories(mc_integration INTERFACE include/)
target_link_libraries(mc_integration INTERFACE -fopenmp)
target_compile_options(mc_integration INTERFACE -fopenmp)
