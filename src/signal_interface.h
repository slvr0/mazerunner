#pragma once 

#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <mutex>
#include <numeric>

#include "task_leader.h"
#include "message_converter.h"

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

class SignalCommInterface
{
    public : 
        SignalCommInterface(std::string pull_adress, TaskLeader* task_leader = nullptr) : 
            rec(ctx2, zmq::socket_type::req), pull_adress_(pull_adress), task_leader_(task_leader) 
        {
                        rec.connect(pull_adress_);              
        }  

        ~SignalCommInterface() {
            rec.close();
            std::cout << "closing receiving socket \n";
        }

        void run()
        {            
            int max_iterations = 50;
            
            zmq::message_t reply;

            bool is_msg_sent = true;
            int start_state = task_leader_->GetState();       

            task_leader_->Reset(); 
        
            std::string msg_ret;
                        
     
                        
            std::map<std::string, float> map_response = {
            {"state" , 3}, 
            {"reward" , 0.0},
            {"train" , 0},
            {"step" , 0}                
            };

            std::map<std::string, int> map_request {
                {"state" , 3}, 
                {"action" , 0}, 
            };

            std::string key_trained = "trained";

            int n_sessions = 0;
            int n_successful_runs = 0;

            std::vector<int> scores;
            for(int i = 0 ; i < 100 ; ++i ) scores.emplace_back(0);
            
            task_leader_->Reset();

            while(true) {
                
                int iteration = -1;  
                
                std:: string response_msg = StateTransitionToJSON(task_leader_->GetStateTransitionInfo());        
                
                bool send_train_req_onbreak = true;

                while(iteration++ < max_iterations) { 
                    //map_response["step"] = iteration;
                    this->send(rec, response_msg.c_str());                    
                    
                    rec.recv (&reply); 

                    std::string rpl = std::string(static_cast<char*>(reply.data()), reply.size());                

                    auto msg_kvp = MessageConverter::ConvertMessageToFloatMap(rpl);  
                    
                    //there could be a received signal that the network has trained. then we should simply update and reset environment 
                    //and send starting metrics. can be done in react aswell!

                    //the signal interface should not logically tell the task leader to send a response message.
                    //the design should be, message comes in, message is passed to task leader,
                    //task leader tells the signal interface to forward a response
                    if(task_leader_) {
                        //if message is , have trained, simply set response to start state and break (without training again ofc)
                        
                        if(msg_kvp.count(key_trained)) {   
                            // map_response["state"] = 3;
                            // map_response["reward"] = 0.0;
                            // map_response["train"] = 0;                        

                            task_leader_->Reset(); //updates state transition 
                            send_train_req_onbreak = false;
                            ++n_sessions;                                                                                 
                            break;                                         
                        }
                        
                        task_leader_->React(msg_kvp, true);  
                        //response_msg = MapToJSON(map_response).c_str(); 
                        response_msg = StateTransitionToJSON(task_leader_->GetStateTransitionInfo());

                        if(task_leader_->GetStatus() == EnvironmentStatus::Finished) {  
                            n_successful_runs++;                            
                            scores[n_sessions % 100] = 1;
                            //task_leader_->DisplaySessionHistory();                           
                            break;
                        }                       
                    }                    
                }

                if(task_leader_->GetStatus() != EnvironmentStatus::Finished){
                    scores[n_sessions % 100] = 0;                    
                }       

                if(send_train_req_onbreak) 
                    task_leader_->OrderTrainSession();               
                
               //std::cout << "sessions  : " << n_sessions << " -- success : " << n_successful_runs << std::endl;
               auto sumscores = std::accumulate(scores.begin(), scores.end(), 0);
               //std::cout << "score : " << (float) n_successful_runs /  n_sessions << std::endl;
               std::cout <<  "AVG score : "  << (float)sumscores/100.0  << std::endl;                           
               
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
        
        std::map<std::string, int> GetDecodedMessage(const std::string msg);       
  
        inline std::string StateTransitionToJSON(StateTransitionInfo transition) {
            std::string ret{"{"};
            
            ret += "state:" + std::to_string(transition.state) + ",";
            ret += "reward:" + std::to_string(transition.reward) + ",";
            ret += "train:" + std::to_string(transition.train) + ",";
            ret += "step:" + std::to_string(transition.step) + ",";
            
            if(transition.mask_transitions.size() != 0 ) {
                ret += "mask:[";

                for (const auto & legal_action : transition.mask_transitions) {
                    ret += std::to_string(legal_action) + "/";
                }

                ret = ret.substr(0, ret.length()-1) + "]}";
            }
            else {
                ret = ret.substr(0, ret.length()-1) + "}";
            }
        
        
            return ret;     
        }
        
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
        TaskLeader* task_leader_ = nullptr;        
};