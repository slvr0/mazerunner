cmake_minimum_required(VERSION 3.10.2)

project(MAZERUNNER)

add_library(
maze_application
src/maze_application.h
src/maze_application.cpp
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE maze_application)

#add_subdirectory(src)


