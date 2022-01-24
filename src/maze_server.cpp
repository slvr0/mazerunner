#include "maze_server.h"

using namespace std;

bool MqttServer::Publish(const std::string & adress, const std::string msg)
{  
	
	mqtt::topic top(cli_, adress, qos_);
	mqtt::token_ptr tok;
	tok = top.publish(msg);

	tok->wait();		   
}

bool MqttServer::Publish(const std::string msg)
{
	mqtt::topic top(cli_, topic_, qos_);
	mqtt::token_ptr tok;
	tok = top.publish(msg);

	tok->wait();
	
}

MqttServer::MqttServer(const std::string adress, const int & qos, const std::string topic) : 
     cli_(adress, ""), qos_(qos), topic_(topic)
{
	cli_.connect()->wait();
}