#include "maze_client.h"

MqttClient::MqttClient(std::string server_adress, std::string client_id, const std::string & topic) : 
    cli_(server_adress, client_id)

{
    
    connOpts_.set_clean_session(false);

    cb_ = std::make_unique<callback>(cli_, connOpts_, topic);		
    
    cli_.set_callback(*cb_.get());

    //connect the client	
    try {
        std::cout << "Connecting to the MQTT server..." << std::flush;
        mqtt::token_ptr tokenptr;
        tokenptr = cli_.connect(connOpts_, nullptr, *cb_.get());
        tokenptr->wait();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "\nERROR: Unable to connect to MQTT server: '"
            << SERVER_ADDRESS << "'" << exc << std::endl;
        
    }
}

MqttClient::~MqttClient()
{
    cli_.disconnect()->wait();
}

void MqttClient::SetCallbackMethod(std::function<void (std::string)> callback_method )
{
    if(cb_)  
        cb_->SetCallbackMethod(callback_method);	
}

bool MqttClient::Publish(const std::string & adress, const std::string msg)
{
    mqtt::topic top(cli_, adress, QOS);
    mqtt::token_ptr tok;
    tok = top.publish(msg);

    tok->wait_for((long)0.00000005);
}
