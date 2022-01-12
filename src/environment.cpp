#include "environment.h"
#include <iostream>
#include <cmath>


TaskLead::TaskLead(MazeStructure* maze_structure) : 
    maze_structure_(maze_structure), current_state_(3)
{
    srand((unsigned) time(0));
}

void TaskLead::Peak()
{
    //whatever, dont need now
}

int TaskLead::Step(const int action)
{
    
}
//random decision making
int TaskLead::Step()
{   
    int step_dir = rand() % 4;
    //step_dir = step_dir == 0 ? 0 : std::pow(step_dir,2);   
    
    if (maze_structure_)
    {   
        std::pair<int,int> state_reward_pair = maze_structure_->GetTransition(current_state_, step_dir);
        if(state_reward_pair.first != -1)
        {
            std::cout << "stepping forward!, from = " << current_state_  << "to  : " << state_reward_pair.first << "\n" ;
            markov_chain_.emplace_back(state_reward_pair.first);
            current_state_ = state_reward_pair.first;
        }
    }

    if(current_state_ == 12) // lol hardcoded now
    {
        std::cout << "we managed to get out of the maze! resetting \n"; 

        std::cout << "path == :: \n";

        for(auto & state_chain : markov_chain_) std::cout <<  state_chain <<  " => ";

        Reset();
    }
}

void TaskLead::Reset()
{
    markov_chain_.clear();
    current_state_ = 3;    
}

void Report()
{

}

MazeEnvironment::MazeEnvironment(const int & level) : 
maze_(std::make_unique<MazeStructure>(level)) 
{
    task_leader_ = std::make_unique<TaskLead> (maze_.get());
}

 int MazeEnvironment::Step()
 {
     return task_leader_->Step();
 }