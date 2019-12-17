cmake_minimum_required(VERSION 3.15)
project(simple_plataform_raylib C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE ON)
add_subdirectory("vendor/raylib")
add_subdirectory("vendor/Chipmunk2D")
add_executable(simple_platform_raylib main.c)
target_link_libraries(simple_platform_raylib PRIVATE raylib chipmunk)
