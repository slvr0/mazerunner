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



