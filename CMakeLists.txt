cmake_minimum_required(VERSION 3.10.2)

project(MAZERUNNER)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src/)
add_subdirectory(paho.mqtt.cpp/)

add_executable(main main.cpp)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

target_include_directories(main PUBLIC)

target_link_libraries(main PRIVATE maze_application maze_comm -std=c++0x -pthread -fno-stack-protector)



#add_subdirectory(src)


