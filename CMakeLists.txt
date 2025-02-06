cmake_minimum_required(VERSION 3.10)
project(hello_window)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_FRAMEWORK_PATH /Library/Frameworks)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED COMPONENTS SDL2)

add_executable(
    hello_window
    glad.c
    triangle.cpp
    classes/shaders/vertex.glsl
    classes/shaders/fragment.glsl
    classes/Triangle.h
)

target_include_directories(hello_window PRIVATE dependencies)
# target_include_directories(hello_window PRIVATE dependencies)
target_link_libraries(hello_window SDL2::SDL2)

