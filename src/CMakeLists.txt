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
    maze_client.h
    maze_client.cpp
    signal_interface.h
    signal_interface.cpp
)


## load in pkg-config support
find_package(PkgConfig)
## use pkg-config to get hints for 0mq locations
pkg_check_modules(PC_ZeroMQ QUIET zmq)


## use the hint from above to find where 'zmq.hpp' is located
find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

## use the hint from about to find the location of libzmq
find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

target_include_directories(maze_comm PRIVATE paho.mqtt.cpp/src/ paho.mqtt.cpp/src/mqtt/)
target_link_libraries(maze_comm PRIVATE paho-mqttpp3 paho-mqtt3as ${ZeroMQ_LIBRARY})

