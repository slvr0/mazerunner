#include "signal_interface.h"

using namespace zmq;

void SignalCommInterface::run()
{  
    zmq::message_t reply; 
    std::string request_msg;

    while(true) { 
        //requests information about current environment                
        request_msg = task_leader_->ReactToZmqDecision(zmqMessageToStdString(reply));

        //sends it over 0mq to the net which will respond taking an action                                  
        this->send(receiver_socket_, request_msg.c_str());  

        //message containing network decision
        receiver_socket_.recv (&reply);               
    }  
    receiver_socket_.close();
}

bool SignalCommInterface::send(zmq::socket_t& socket, const char* data) const {
    size_t size = strlen(data);
    zmq::message_t message(size);
    std::memcpy (message.data(), data, size);
    bool rc = socket.send(message);
    return (rc);
}


