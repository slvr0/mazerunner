#include "signal_interface.h"

// DecodedMessage SignalCommInterface::GetDecodedMessage(const std::string message) {
//     DecodedMessage dmsg;

//     //make these available globally later, topics are currently not used.
//     const std::string rec_action_taken = "nn_send";
//     const std::string some_other_response = "nn_whatever";

//     dmsg.decoded_message_ = message;
//     return dmsg;
    

//     auto st_contains = [&](std::string substring) { 
//         return message.find(substring) != std::string::npos; };

//     if(st_contains(rec_action_taken)) {
//         dmsg.msg_type_ = MessageType::RECEIVE_ACTION_TAKEN;
//         dmsg.decoded_message_ = message.substr(rec_action_taken.length());
//     }
//     else if(st_contains(some_other_response)){
//         dmsg.msg_type_ = MessageType::INVALID;
//         dmsg.decoded_message_ = message.substr(some_other_response.length());
//     }    

//     return dmsg;
// }


// DecodedMessage SignalCommInterface::GetDecodedMessage(zmq::message_t message) {
//     return GetDecodedMessage(std::string(static_cast<char*>(message.data()), message.size()));
// }

std::map<std::string, int> SignalCommInterface::GetDecodedMessage(const std::string msg)
        {
            std::map<std::string, int> kvp;
            size_t start_curl = msg.find("{");
            size_t end_curl = msg.find("}");

            std::vector<int> vseps = find_all_char_positions_in_string(msg, ',');
            std::vector<int> seps = find_all_char_positions_in_string(msg, ':');    


            if(seps.size() == 0) {
            std::cout << "special exit 1 in react method\n";
            return kvp;
            }

            //this should be its own algorithm, nothing to do with the taskleader 
            if(vseps.size() == 0)  //first element in a single item list
                kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, end_curl));
                else 
                {
                    for(int i = 0 ; i < seps.size() ; ++i) {
                    if(i == 0) 
                    {
                        kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, vseps[0])); //first element
                    }
                    else 
                    {
                        if(i == vseps.size()) //last element
                        {                   
                            kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                                std::stof(msg.substr(seps[i]+1, seps[i]+1 - end_curl - 1));
                        }
                        else //mid elements
                        { 
                            kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                                std::stof(msg.substr(seps[i] + 1, vseps[i] - seps[i] - 1));
                    
                }                  
            }

            }
        }
        return kvp;
        }

