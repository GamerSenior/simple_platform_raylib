cmake_minimum_required(VERSION 3.15)
project(simple_plataform_raylib C)

add_library(raylib SHARED IMPORTED)
set_target_properties(raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.so"
        INTERFACE_INCLUDE_LOCATION "${CMAKE_SOURCE_DIR}}/include/raylib.h"
        )

set(CMAKE_C_STANDARD 11)
target_link_libraries()
add_executable(simple_plataform_raylib main.c)