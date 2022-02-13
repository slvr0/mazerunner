#pragma once
#include <map>
#include <vector>
#include <iostream>

#include "env_model.h"
#include "custom_environment.h"

//should probably be a fixed state representation of 0/1/2 dimensions. And the msg converter needs to have a fixed input of dimension.

class TaskLeader
{
public : 
    TaskLeader(CustomEnvironment* custom_env = nullptr);

    void  React(std::map<std::string, float> input_args,  bool mask_legal = true);

    //returns startstate
    int Reset();
    void Report();

    inline StateTransitionInfo GetStateTransitionInfo() const {
        return transition_;
    }

    constexpr int GetEnvironmentOutputDimension () const {
        return env_->GetOutputDimension();
    }
    
    inline std::vector<int> RequestLegalTransitions(StateTransitionInfo state_transition_info){
        return env_->GetLegalTransitions(state_transition_info);
    }   

    inline void SetEnvironmentInterface(CustomEnvironment* env) { env_ = env;}
    
    inline EnvironmentStatus GetStatus() const 
    {
        return status_;
    }

    void OrderTrainSession()
    {
        transition_.train = 1;
    }
    
    inline int GetState() const {
        return current_state_;
    }

    void DisplaySessionHistory(bool log = false);
    
    private: 
    CustomEnvironment* env_ = nullptr;
    std::vector<int> markov_chain_;  
    int current_state_;  
    EnvironmentStatus status_;
    int current_step_;

    StateTransitionInfo transition_;
   
};