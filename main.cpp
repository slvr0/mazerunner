#include <iostream>

#include "src/maze_application.h"
#include "src/maze_server.h"


#include "src/maze_client_example.h"

using namespace std;

int main(int argc, char** argv) 
{

	//MazeApplication maze_app;
    // MqttServer server;
    
    // server.Publish("maze", "hello world!");

	std::string client_topic = "nn_response";

	MqttClientTest mqtt_client(SERVER_ADDRESS, CLIENT_ID, client_topic);

	TaskLead tasklead;

	tasklead.SetMqttClient(mqtt_client);	

	while (std::tolower(std::cin.get()) != 'q')
	{

	}

    return 0;
}
