#include "task_leader.h"

#include "message_converter.h"
#include "utils.h"

TaskLeader::TaskLeader(CustomEnvironment* env) : 
    env_(env), current_state_(3)
{
    srand((unsigned) time(0));
}

std::string TaskLeader::ReactToZmqDecision(std::string message) {
    if(!session_initiated_) {
        this->Reset();
        session_initiated_ = true;                
    }
    else {
        auto args = MessageConverter::ConvertJSONToFloatMap(message);
        this->React(args);               
    }

    return this->StateTransitionToJSON();
}

void TaskLeader::React(std::map<std::string, float> input_args,  bool mask_legal)
{  
    if(DEBUG)
        PPrintMap(input_args);

    //input validation
    if(!input_args.count(KEY_STATE) || !input_args.count(KEY_ACTION)) {      
        return ;
    }
    int env_state   = static_cast<int> (input_args[KEY_STATE]); 
    int env_action  = static_cast<int> (input_args[KEY_ACTION]);   

    //this transition update should include done / info flags too.
    auto get_transition = env_->GetTransition(transition_ , env_action);  

    if(DEBUG)
        std::cout << "moving from state : " << env_state << " with action " << env_action << "results in " << get_transition.first << std::endl;
    
    //update current state
    current_state_ = get_transition.first;

    //save trajectory in session history
    markov_decision_chain_.emplace_back(current_state_);

    //update state transition params
    transition_.state   = get_transition.first;
    transition_.reward  = get_transition.second;
    transition_.step    += 1;
         
    if(mask_legal) {  
        transition_.mask_transitions = env_->GetLegalActions(transition_);
    }       
        
    if(get_transition.first == 12) 
        status_ = EnvironmentStatus::Finished;   
} 

void TaskLeader::Reset() 
{
    status_ = EnvironmentStatus::Playing;
    markov_decision_chain_.clear();
    current_state_ = 3;   
    markov_decision_chain_.emplace_back(current_state_); 
    current_step_ = 0;

    //set state transition info 
    transition_.state = 3;
    transition_.reward = 0;
    transition_.step = 0;
    transition_.train = 0;
    transition_.mask_transitions = this->RequestLegalActions(transition_);
}

void TaskLeader::DisplaySessionHistory(bool log){

    bool print_gamechain = true;
    bool print_gameresultscore = false;

    if(print_gamechain) { 
        std::cout << "game history is :: \n";
        for(const auto & element : markov_decision_chain_) 
        {
            std::cout << " state : " << element;
        } 
        std::cout << std::endl;    
    }
    if(print_gameresultscore) {
        const size_t n_steps = markov_decision_chain_.size();
        if(current_state_ == 12) {
            std::cout << "score : " << (float)1.0 / n_steps << std::endl;
        }
        else {
            std::cout << "score : " << 0 << " final state : " << current_state_ << std::endl;
        }
    }
}

std::string TaskLeader::StateTransitionToJSON(StateTransitionInfo transition) const {
    std::string ret{"{"};

    ret += "state:" + std::to_string(transition.state) + ",";
    ret += "reward:" + std::to_string(transition.reward) + ",";
    ret += "train:" + std::to_string(transition.train) + ",";
    ret += "step:" + std::to_string(transition.step) + ",";

    if(transition.mask_transitions.size() != 0 ) {
        ret += "mask:[";

        for (const auto & legal_action : transition.mask_transitions) {
            ret += std::to_string(legal_action) + "/";
        }

        ret = ret.substr(0, ret.length()-1) + "]}";
    }
    else {
        ret = ret.substr(0, ret.length()-1) + "}";
    }

    return ret;             
}

std::string TaskLeader::StateTransitionToJSON() const {
    return this->StateTransitionToJSON(transition_);
}