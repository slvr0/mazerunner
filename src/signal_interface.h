#pragma once 

#include <iostream>
#include <thread>

#include "task_leader.h"
#include "message_converter.h"

#include "zmq.hpp"

class SignalCommInterface
{
public : 
    SignalCommInterface(std::string pull_adress, TaskLeader* task_leader = nullptr) : 
        receiver_socket_(context_, zmq::socket_type::req), pull_adress_(pull_adress), task_leader_(task_leader) 
    {
        receiver_socket_.connect(pull_adress_);              
    }  

    ~SignalCommInterface() {
        receiver_socket_.close();        
    }

    void run();

    bool send(zmq::socket_t& socket, const char* data) const;

    inline std::thread RunThreaded() {
        return std::thread([=] { run(); });
    }    

private : 

    inline std::string zmqMessageToStdString(zmq::message_t &message) {
        return std::string(static_cast<char*>(message.data()), message.size()); 
    }

    zmq::context_t context_;
    zmq::socket_t receiver_socket_;           
    std::string push_adress_;
    std::string pull_adress_;
    TaskLeader* task_leader_ = nullptr;        
};
