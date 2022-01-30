#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include "maze_client.h"
#include "maze_server.h"
#include <cassert>

#include "maze_structure.h"



enum class EnvironmentStatus
{
    Finished = 0,
    Playing = 1
};

inline std::vector<int> find_all_char_positions_in_string(std::string sample, char findIt)
{
    std::vector<int> characterLocations;
    for(int i =0; i < sample.size(); i++)
        if(sample[i] == findIt)
            characterLocations.push_back(i);

    return characterLocations;
}

//  for (auto const &pair: kvp) {
//         std::cout << pair.first << " : " << pair.second << "\n";
//         }


class TaskLead
{
public : 
    TaskLead(MazeStructure* maze_structure = nullptr);

    void Peak();

    //failry vague method name, the taskleader updates environment status with help from a message coming from the neural net in zmq message.
    void  React(std::map<std::string, int> input_args, std::map<std::string, int> &map_response);

    //returns startstate
    int Reset();
    void Report();

    inline void SetEnvironmentInterface(MazeStructure* maze_structure) { maze_structure_ = maze_structure;}
    
    inline EnvironmentStatus GetStatus() const 
    {
        return status_;
    }
    
    inline int GetState() const {
        return current_state_;
    }

    void DisplaySessionHistory(bool log = false);

    
    private: 
    MazeStructure* maze_structure_ = nullptr;
    std::vector<int> markov_chain_;  
    int current_state_;  
    EnvironmentStatus status_;
    int current_step_;
   
};

class MazeEnvironment
{
public :
    MazeEnvironment(const int & level); 

    int Step();

    inline TaskLead* GetTaskLeader() const {
        return task_leader_.get();
    }
    void run();
    int Reset();

private: 
    std::unique_ptr<TaskLead> task_leader_ = nullptr;
    std::unique_ptr<MazeStructure> maze_ = nullptr;  
};