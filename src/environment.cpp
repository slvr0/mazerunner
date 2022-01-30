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


//sets up connection between pythons comm telling c where it is and what action it chooses,  
//subsequent response (new state/reward) is sent back to a respond topic.
 void TaskLead::React(std::map<std::string, int> input_args, std::map<std::string, int> & map_response)
{      
    //std::cout << "taskleader reacting to message " << msg << std::endl;
    bool debug = false;
    
    std::string key_state = "state";
    std::string key_action = "action";
    std::string key_trained = "trained";

    //this should be independly logic on environment, override this with virtual method later

    if(!input_args.count(key_state) || !input_args.count(key_action)) {
        std::cout << "special exit 2 in react method\n";
        return ;
    }

    int env_state = static_cast<int> (input_args[key_state]); 
    int env_action = static_cast<int> (input_args[key_action]);   

    //this transition update should include done / info flags too.
    auto get_transition = maze_structure_->GetTransition(env_state ,env_action);  

    if(debug)
        std::cout << "moving from state : " << env_state << " with action " << env_action << "results in " << get_transition.first << std::endl;
    
    //update current state
    current_state_ = get_transition.first;

    //save it in the session history
    markov_chain_.emplace_back(current_state_);


    map_response["state"] = get_transition.first;
    map_response["reward"] = get_transition.second;
    
    
    if(get_transition.first == 12) 
        status_ = EnvironmentStatus::Finished;       

} 

int TaskLead::Reset()
{
    status_ = EnvironmentStatus::Playing;
    markov_chain_.clear();
    current_state_ = 3;   
    markov_chain_.emplace_back(current_state_); 
    current_step_ = 0;
}

void TaskLead::DisplaySessionHistory(bool log){

    bool print_gamechain = true;
    bool print_gameresultscore = false;

    if(print_gamechain) { 
        std::cout << "game history is :: \n";
        for(const auto & element : markov_chain_) 
        {
            std::cout << " state : " << element;
        } 
        std::cout << std::endl;    
    }
    if(print_gameresultscore) {
        const size_t n_steps = markov_chain_.size();
        if(current_state_ == 12) {
            std::cout << "score : " << (float)1.0 / n_steps << std::endl;
        }
        else {
            std::cout << "score : " << 0 << " final state : " << current_state_ << std::endl;
        }
    }
}

MazeEnvironment::MazeEnvironment(const int & level) : 
maze_(std::make_unique<MazeStructure>(level))


{

    task_leader_ = std::make_unique<TaskLead> (maze_.get());

    
}






//actually not really needed as of now, we start the chain comm right away in signal interface, awful design needs improv.
void MazeEnvironment::run()
{
    while (std::tolower(std::cin.get()) != 'q')
    {
        bool done = false;

        int state = Reset();

        while(!done)
        {
            
            //praaay barr

            
        }
    }
}

int MazeEnvironment::Reset()
{
    return task_leader_->Reset();
}


int MazeEnvironment::Step()
{
    //return task_leader_->Step();
}