#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <memory>
#include <vector>
#include <functional>

#include "maze_client_example.h"

#include "maze_structure.h"

//decides where to go next,
class TaskLead
{
public : 
    TaskLead(MazeStructure* maze_structure = nullptr);

    void Peak();
    int Step(const int action);
    int Step();
    void React(const std::string decision)
    {
        std::cout << "React called! with msg " << decision << std::endl ;
    } // binds a message on a mqtt topic that callbacks react so we can take decision on it.

    void Reset();
    void Report();

    inline void SetEnvironmentInterface(MazeStructure* maze_structure) { maze_structure_ = maze_structure;}
    inline void SetMqttClient(MqttClientTest& client) {
        client_ = &client;

        //std::function<void (std::string)> func_ptr = &this->React();

        auto react_lambda = [&](const std::string msg) 
        {
            this->React(msg);
        };

        client_->SetCallbackMethod(react_lambda);
    }
    
    private: 
    MazeStructure* maze_structure_ = nullptr;
    std::vector<int> markov_chain_;  
    int current_state_;    
    MqttClientTest* client_;   
   
   
};


class MazeEnvironment
{
public :
    MazeEnvironment(const int & level); 

    int Step();


private: 
    std::unique_ptr<TaskLead> task_leader_ = nullptr;
    std::unique_ptr<MazeStructure> maze_ = nullptr;  
	
};