#include <iostream>
#include <map>
#include <string>

#include "src/maze_application.h"
#include "src/maze_server.h"

#include "src/environment.h"
#include "src/signal_interface.h"


#include <thread>
// class Wrapper {
//    public:
//       void member1() {
// 		  while(true)
//           	std::cout << "i am " << member11  << std::endl;
//       }
//       void member2(const char *arg1, unsigned arg2) {

// 		  while(true)
//           	std::cout << "i am "  << member22 << "and my first arg is (" << arg1 << ") and second arg is (" << arg2 << ")" << std::endl;
//       }
//       std::thread member1Thread() {
//           return std::thread([=] { member1(); });
//       }
//       std::thread member2Thread(const char *arg1, unsigned arg2) {
//           return std::thread([=] { member2(arg1, arg2); });
//       }

// 	private : 
// 	int member11 = 1;
// 	int member22 = 2;
// };



int main(int argc, char** argv) 
{
	// std::string test_topic = "nn_send_1";
	// MqttClient mqtt_client(SERVER_ADDRESS, CLIENT_ID, test_topic);
	// MazeStructure* maze = new MazeStructure(0);

	// TaskLead tasklead{maze};
	// tasklead.SetMqttClient(mqtt_client);

	// std::string test_msg = "{state:3,action:0}";
	// tasklead.React(test_msg);


	// MazeApplication maze_app;
	// maze_app.run();

	
    // MqttServer server;
    
    // server.Publish("maze", "hello world!");

	//std::string client_topic = "nn_response";

	// MqttClientTest mqtt_client(SERVER_ADDRESS, CLIENT_ID, client_topic);

	// TaskLead tasklead;

	// tasklead.SetMqttClient(mqtt_client);	

	// while (std::tolower(std::cin.get()) != 'q')
	// {

	// }

	
//run this

	std::string zmq_pull_msg_adress = "tcp://127.0.0.1:5511";
	std::string zmq_push_msg_adress = "tcp://127.0.0.1:5557";

	int level = 0;
	MazeEnvironment maze_env(level);

	ZMQSender sigcomm_sender(zmq_pull_msg_adress);

	SignalCommInterface sigcomm_interface(&sigcomm_sender, zmq_push_msg_adress, maze_env.GetTaskLeader());

	std::thread sender_thread = sigcomm_sender.RunThread();
	std::thread sigcomm_thread = sigcomm_interface.RunThread();

	sender_thread.join();
	sigcomm_thread.join();

	
    return 0;
}
