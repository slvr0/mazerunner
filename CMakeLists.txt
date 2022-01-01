cmake_minimum_required(VERSION 3.10.2)

project(MAZERUNNER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(
maze_application
src/maze_application.h
src/maze_application.cpp
src/maze_structure.h
src/maze_structure.cpp
src/environment.cpp
)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE maze_application)

#add_subdirectory(src)


