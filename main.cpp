#include <iostream>
#include <map>
#include <string>
#include <thread>

#include "src/signal_interface.h"
#include "src/env_organizer.h"

int main(int argc, char** argv) 
{	std::string zmq_pull_msg_adress = "tcp://127.0.0.1:5511";
	std::string zmq_push_msg_adress = "tcp://127.0.0.1:5557";
	std::string zmq_request_adress 	= "tcp://127.0.0.1:5533";

	int level = 0;
	EnvironmentOrganizer environment(level);
	SignalCommInterface sigcomm_interface(zmq_request_adress, environment.GetTaskLeader());

	std::thread sigcomm_thread = sigcomm_interface.RunThreaded();

	while (std::tolower(std::cin.get()) != 'q')
	{
		
	}

	sigcomm_thread.join();
	
    return 0;
}
