cmake_minimum_required(VERSION 3.22.1)
project(kairo68 LANGUAGES C CXX)

get_filename_component(KAIRO68_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../.." ABSOLUTE)
include("${KAIRO68_ROOT}/shared/cmake/KairoMachine.cmake")

# The X68000 on MAME's x68k driver, driven one frame at a time (core/).
add_subdirectory("${KAIRO68_ROOT}/core" core)

kairo_machine_library(kairo68 x68k_core.cpp)
target_link_libraries(kairo68 PRIVATE kairo68_core)
