#include "task_leader.h"

TaskLeader::TaskLeader(CustomEnvironment* env) : 
    env_(env), current_state_(3)
{
    srand((unsigned) time(0));
}

void TaskLeader::React(std::map<std::string, float> input_args,  bool mask_legal)
{  
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
    auto get_transition = env_->GetTransition(transition_ , env_action);  

    if(debug)
        std::cout << "moving from state : " << env_state << " with action " << env_action << "results in " << get_transition.first << std::endl;
    
    //update current state
    current_state_ = get_transition.first;

    //save it in the session history
    markov_chain_.emplace_back(current_state_);

    transition_.state = get_transition.first;
    transition_.reward = get_transition.second;
    transition_.step += 1;
         
    if(mask_legal) {  
        transition_.mask_transitions = env_->GetLegalTransitions(transition_);

    }       
        
    if(get_transition.first == 12) 
        status_ = EnvironmentStatus::Finished;      

} 

int TaskLeader::Reset()
{
    status_ = EnvironmentStatus::Playing;
    markov_chain_.clear();
    current_state_ = 3;   
    markov_chain_.emplace_back(current_state_); 
    current_step_ = 0;

    //set state transition info 
    transition_.state = 3;
    transition_.reward = 0;
    transition_.step = 0;
    transition_.train = 0;
    transition_.mask_transitions = this->RequestLegalTransitions(transition_);
}

void TaskLeader::DisplaySessionHistory(bool log){

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