#pragma once 

#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <mutex>

#include "environment.h"

#include "zmq.hpp"

using namespace zmq;

enum class MessageType
{
    INVALID = 0,
    RECEIVE_ACTION_TAKEN = 1, //the neural net informs what action it has performed , we reply with new state and feedback on decision.
};

struct DecodedMessage
{
    std::string decoded_message_;
    MessageType msg_type_;
};


class ZMQSender 
{
    public : 
    ZMQSender(const std::string adress) : adress_(adress), sender(ctx, zmq::socket_type::push)  {        
     
        sender.connect(adress);
    
    }

    void run()
    {
        while(true) {

            
            if(!message_stack_.empty()) {
                
                zmq_sender_mtx_.lock();
                auto data_element = message_stack_.back();
                size_t size = strlen(data_element);
                zmq::message_t message(size);
                std::memcpy (message.data(), data_element, size);
                bool rc = sender.send (message);
                message_stack_.pop_back(); 
                zmq_sender_mtx_.unlock();

                //std::cout << "sending message " << data_element << "on port " << adress_ << std::endl;                        
            } 
        }

    }

    std::thread RunThread() {
        return std::thread([=] { run(); });
    }
    
    inline void QueMessage(const char* msg) {
        zmq_sender_mtx_.lock();
        message_stack_.emplace_back(msg);
        zmq_sender_mtx_.unlock();
    }

    private :
    std::string adress_;
    std::vector<const char*> message_stack_;
    zmq::context_t ctx;
    zmq::socket_t sender;
    std::mutex zmq_sender_mtx_;   
};


class SignalCommInterface
{
    public : 
        SignalCommInterface(ZMQSender* sender, std::string pull_adress, TaskLead* task_leader = nullptr) : 
            sender_(sender) , rec(ctx2, zmq::socket_type::pull), pull_adress_(pull_adress), task_leader_(task_leader) 
        {
                        rec.bind(pull_adress_);              
        }  

        void run()
        {
            zmq::message_t reply;

            bool is_msg_sent = true;
            int start_state = task_leader_->GetState();

            std::cout << "starting comm... sending start message to initiate env progress\n"; 

           
            std::string response_msg = "{state:3}";  

            std::map<std::string, int> map_response = {
                {"state" , start_state}, 
                {"reward" , 0}

            };

            std::string msg_ret;
            while(true) {                  
                sender_->QueMessage(response_msg.c_str());                   
 
                rec.recv (&reply); 
             
                std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());

                DecodedMessage decoded_msg = GetDecodedMessage(rpl);

                //the signal interface should not logically tell the task leader to send a response message.
                //the design should be, message comes in, message is passed to task leader,
                //task leader tells the signal interface to forward a response
                if(task_leader_)
                    //std::cout << "taskleader reacting to nn response \n"; 
                    task_leader_->React(decoded_msg.decoded_message_, map_response);  
                    response_msg = MapToJSON(map_response).c_str();  
                           

            }

        }  

        std::thread RunThread() {
            return std::thread([=] { run(); });
        }

        bool send(zmq::socket_t& socket, const char* data) const {
            size_t size = strlen(data);
            zmq::message_t message(size);
            std::memcpy (message.data(), data, size);
            bool rc = socket.send (message);
            return (rc);
        }

        DecodedMessage GetDecodedMessage(const std::string message); 
        DecodedMessage GetDecodedMessage(zmq::message_t message); 

   

        template <typename A>
        inline std::string MapToJSON(std::map<std::string, A> dict_)
        {
            std::string ret{"{"};

            for(auto & [k, v] : dict_){
            ret += k  + ":" + std::to_string(v) + ",";
            }

            ret = ret.substr(0, ret.length()-1) + "}";
            return ret;
        }



    private : 
        zmq::context_t ctx2;
        zmq::socket_t rec;           
        std::string push_adress_;
        std::string pull_adress_;
        TaskLead* task_leader_ = nullptr;
        ZMQSender* sender_;
};