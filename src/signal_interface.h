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
        //sender.setsockopt(ZMQ_SNDHWM, 0, 0);
        sender.connect(adress);       
    }
    ~ZMQSender() {
            sender.close();
            std::cout << "closing sender socket \n";
        }

    void run()
    {
        while(true) {

            zmq::message_t message; 
            if(!message_stack_.empty()) {
                
                zmq_sender_mtx_.lock();
                auto data_element = message_stack_.back();
                size_t size = strlen(data_element);
                message.rebuild(size);
                std::memcpy (message.data(), data_element, size);              
                bool rc = sender.send(message);
                message_stack_.pop_back(); 
                zmq_sender_mtx_.unlock();

                //std::cout << "messages in buffer : " << message_stack_.size() << std::endl;

                //std::cout << "sending message " << data_element << "on port " << adress_ << std::endl;                        
            } 
        }
        sender.close();
    }

    std::thread RunThread() {
        return std::thread([=] { run(); });
    }
    
    inline void QueMessage(const char* msg) {
        zmq_sender_mtx_.lock();
        message_stack_.emplace_back(msg);
        //std::cout << msg << std::endl;
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
            sender_(sender) , rec(ctx2, zmq::socket_type::req), pull_adress_(pull_adress), task_leader_(task_leader) 
        {
                        rec.connect(pull_adress_);              
        }  

        ~SignalCommInterface() {
            rec.close();
            std::cout << "closing receiving socket \n";
        }

        void run()
        {
            
            int max_iterations = 30;

            zmq::message_t reply;

            bool is_msg_sent = true;
            int start_state = task_leader_->GetState();

            //std::cout << "starting comm... sending start message to initiate env progress\n"; 

            task_leader_->Reset(); 
        
            std::string msg_ret;
            
            std::map<std::string, int> map_response = {
            {"state" , 3}, 
            {"reward" , 0},
            {"train" , 0},
            {"step" , 0}
            };

            std:: string response_msg = MapToJSON(map_response).c_str(); 

            while(true) {
                int iteration = -1;
                

                while(iteration++ < max_iterations) { 
                    map_response["step"] = iteration;
                    this->send(rec, response_msg.c_str());
                    
                    rec.recv (&reply);                     
                    
                    PrintMap(map_response);
                    
                    std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());                

                    DecodedMessage decoded_msg = GetDecodedMessage(rpl);

                    //there could be a received signal that the network has trained. then we should simply update and reset environment 
                    //and send starting metrics. can be done in react aswell!

                    //the signal interface should not logically tell the task leader to send a response message.
                    //the design should be, message comes in, message is passed to task leader,
                    //task leader tells the signal interface to forward a response
                    if(task_leader_) { 


                        task_leader_->React(decoded_msg.decoded_message_, map_response);  

                        //response_msg = "{state:3,reward:1}";
                        response_msg = MapToJSON(map_response).c_str(); 

                        if(task_leader_->GetStatus() == EnvironmentStatus::Finished) {  
                            std::cout << "we finished the maze! \n";                        
                            break;
                        }                       
                    }
                    
                }

                map_response["train"] = 1;

                
            
            
            //important, final state and reward is sent to nn before concluding session
            //task_leader_->DisplaySessionHistory();

            //taskleader doesnt reset environment here, he awaits for confirmation that nn have trained , then resets in react method.
            //task_leader_->Reset(); 

            // auto state = map_response["state"];
            // auto reward = map_response["reward"];    

            // std:: cout << state << std::endl;            

            // response_msg = "{state:" + std::to_string(state) + "," + "reward:" + std::to_string(reward) + ",train:1}";
            
                
            }  
            rec.close();
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

        template <typename A>
        inline void PrintMap(std::map<std::string , A> dict) 
        {
            //std::cout << "dict entries : " << dict.size() << std::endl;

            std::cout << "\n";
            for(auto & [k, v] : dict){
                std::cout << k << ":" << std::to_string(v) << std::endl;
            }
            std::cout << "\n";
        }



    private : 
        zmq::context_t ctx2;
        zmq::socket_t rec;           
        std::string push_adress_;
        std::string pull_adress_;
        TaskLead* task_leader_ = nullptr;
        ZMQSender* sender_;
};