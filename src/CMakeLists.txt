cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#user adds all dependenciy files under this library
add_library(user_environment
    custom_environment.h
    custom_environment.cpp
) 

add_library(
    env_interaction
    env_model.h
    env_model.cpp
    task_leader.h
    task_leader.cpp    
    env_organizer.h
    env_organizer.cpp    
)   

add_library(
    signal_comm    
    signal_interface.h
    signal_interface.cpp
)

add_library(
    utils    
    utils.h
    utils.cpp

)

find_package(PkgConfig)

pkg_check_modules(PC_ZeroMQ QUIET zmq)

find_path(ZeroMQ_INCLUDE_DIR
        NAMES zmq.hpp
        PATHS ${PC_ZeroMQ_INCLUDE_DIRS}
        )

find_library(ZeroMQ_LIBRARY
        NAMES zmq
        PATHS ${PC_ZeroMQ_LIBRARY_DIRS}
        )

target_include_directories(signal_comm PRIVATE paho.mqtt.cpp/src/ paho.mqtt.cpp/src/mqtt/)
target_link_libraries(signal_comm PRIVATE env_interaction paho-mqttpp3 paho-mqtt3as ${ZeroMQ_LIBRARY})

target_link_libraries(env_interaction PRIVATE utils)

