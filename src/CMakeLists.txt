cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find_package(Qt5 COMPONENTS Widgets REQUIRED)
# find_package(Qt5 COMPONENTS Core REQUIRED)
# find_package(Qt5 COMPONENTS Network REQUIRED)

# # set up a mapping so that the Release configuration for the Qt imported target is
# # used in the COVERAGE CMake configuration.
# set_target_properties(Qt5::Core PROPERTIES MAP_IMPORTED_CONFIG_COVERAGE "RELEASE")

add_library(
    maze_application
    maze_application.h
    maze_application.cpp
    maze_structure.h
    maze_structure.cpp
    environment.cpp
)   

add_library(maze_comm
    maze_server.h
    maze_server.cpp
    maze_client_example.h
    maze_client_example.cpp
)

target_include_directories(maze_comm PRIVATE paho.mqtt.cpp/src/ paho.mqtt.cpp/src/mqtt/)
target_link_libraries(maze_comm PRIVATE paho-mqttpp3 paho-mqtt3as)

