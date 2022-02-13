#include "env_organizer.h"
#include <iostream>
#include <cmath>

EnvironmentOrganizer::EnvironmentOrganizer(const int & level) : 
env_(std::make_unique<CustomEnvironment>(level))
{
    task_leader_ = std::make_unique<TaskLeader> (env_.get());    
}

void EnvironmentOrganizer::run()
{
    while (std::tolower(std::cin.get()) != 'q')
    {
        bool done = false;

        int state = Reset();

        while(!done)
        {            
           
        }
    }
}

int EnvironmentOrganizer::Reset()
{
    return task_leader_->Reset();
}

int EnvironmentOrganizer::Step()
{
    //return task_leader_->Step();
}