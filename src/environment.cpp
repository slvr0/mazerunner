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
//sets up connection between pythons comm telling c where it is and what action it chooses,  
//subsequent response (new state/reward) is sent back to a respond topic.
 void TaskLead::React(const std::string msg, std::map<std::string, int> & map_response)
{        
    //std::cout << "taskleader reacting to message " << msg << std::endl;
    bool debug = false;

    size_t start_curl = msg.find("{");
    size_t end_curl = msg.find("}");
    
    std::vector<int> vseps = find_all_char_positions_in_string(msg, ',');
    std::vector<int> seps = find_all_char_positions_in_string(msg, ':');    

    std::map<std::string, float> kvp;

    if(seps.size() == 0) {
        std::cout << "special exit 1 in react method\n";
        return ;
    }

    //this should be its own algorithm, nothing to do with the taskleader 
    if(vseps.size() == 0)  //first element in a single item list
        kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, end_curl));
    else 
    {
        for(int i = 0 ; i < seps.size() ; ++i) {
            if(i == 0) 
            {
                kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, vseps[0])); //first element
            }
            else 
            {
                if(i == vseps.size()) //last element
                {                   
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stof(msg.substr(seps[i]+1, seps[i]+1 - end_curl - 1));
                }
                else //mid elements
                { 
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stof(msg.substr(seps[i] + 1, vseps[i] - seps[i] - 1));
                    
                }                  
            }

        }
    }

    //this should be independly logic on environment, override this with virtual method later

    std::string key_state = "state";
    std::string key_action = "action";
    std::string key_trained = "trained";

    if(kvp.count(key_trained)) {
       map_response["state"] = 3;
       map_response["reward"] = 0;
       map_response["train"] = 0;

       this->Reset(); 
       return;
    }

    if(!kvp.count(key_state) || !kvp.count(key_action)) {
        std::cout << "special exit 2 in react method\n";
        return ;
    }

    //assert(kvp.count(key_state) && kvp.count(key_action) && "error , no key / action was passes as argument in message to task leader \n");

    int env_state = static_cast<int> (kvp[key_state]); 
    int env_action = static_cast<int> (kvp[key_action]);   

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
        //map_response["train"] = 1;

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

int TaskLead::Reset()
{
    status_ = EnvironmentStatus::Playing;
    markov_chain_.clear();
    current_state_ = 3;   
    markov_chain_.emplace_back(current_state_); 
    current_step_ = 0;
}

void TaskLead::DisplaySessionHistory(bool log){

    bool print_gamechain = false;
    bool print_gameresultscore = true;

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
    return task_leader_->Step();
}