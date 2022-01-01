#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <memory>
#include <vector>


#include "maze_structure.h"

//decides where to go next,
class TaskLead
{
public : 
    TaskLead(MazeStructure* maze_structure);

    void Peak();
    int Step(const int action);
    int Step();
    void Reset();
    void Report();

    private: 
    MazeStructure* maze_structure_ = nullptr;
    std::vector<int> markov_chain_;  
    int current_state_;
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