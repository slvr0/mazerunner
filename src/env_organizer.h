#pragma once
#include <memory>

#include "maze_structure.h"
#include "custom_environment.h"
#include "task_leader.h"

class EnvironmentOrganizer
{
public :
    EnvironmentOrganizer(const int & level); 
    
    int Step();

    inline TaskLeader* GetTaskLeader() const {
        return task_leader_.get();
    }
    void run();
    int Reset();

private: 
    std::unique_ptr<TaskLeader> task_leader_ = nullptr;
    std::unique_ptr<CustomEnvironment> env_ = nullptr;  
};