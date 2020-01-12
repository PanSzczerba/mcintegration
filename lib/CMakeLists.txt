cmake_minimum_required(VERSION 3.10)
find_package(OpenMP REQUIRED COMPONENTS CXX C)

add_library(mc_integration INTERFACE)
target_include_directories(mc_integration INTERFACE include/)
target_link_libraries(mc_integration INTERFACE ${OpenMP_CXX_FLAGS})
target_compile_options(mc_integration INTERFACE ${OpenMP_CXX_FLAGS})
