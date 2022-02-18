#pragma once
#include <map>
#include <vector>
#include <iostream>

#include "env_model.h"
#include "custom_environment.h"

//should probably be a fixed state representation of 0/1/2 dimensions. And the msg converter needs to have a fixed input of dimension.

#define DEBUG 0

#define KEY_STATE "state"
#define KEY_ACTION "action"
#define KEY_TRAINED "trained"

class TaskLeader
{
public : 
    TaskLeader(CustomEnvironment* custom_env = nullptr);

    //updates state transition from a decision response
    void React(std::map<std::string, float> input_args,  bool mask_legal = true);
    
    //entry point to React method
    std::string ReactToZmqDecision(std::string message);

    //resets the environment and parameters in transition_
    void Reset();

    inline StateTransitionInfo GetStateTransitionInfo() const {
        return transition_;
    }

    constexpr int GetEnvironmentOutputDimension () const {
        return env_->GetOutputDimension();
    }
    
    //requests environments legal actions in environment, forwarded as a mask argument in json
    inline std::vector<int> RequestLegalActions(StateTransitionInfo state_transition_info) {
        return env_->GetLegalActions(state_transition_info);
    }   

    inline void SetEnvironmentInterface(CustomEnvironment* env) { 
        env_ = env;
    }
    
    inline EnvironmentStatus GetStatus() const {    
        return status_;
    } 

    inline void OrderTrainSession() {
        transition_.train = 1;
    }
    
    inline int GetState() const {
        return current_state_;
    }

    template <typename T> //linking error from utils.cpp
    void PPrintMap(std::map<std::string, T> __map){
        std::cout << "\n";
        for(auto & [k, v] : __map){
            std::cout << k << ":" << std::to_string(v) << std::endl;
        }
        std::cout << "\n";
    }

    std::string StateTransitionToJSON(StateTransitionInfo transition) const;
    std::string StateTransitionToJSON() const;

    void DisplaySessionHistory(bool log = false);
    
private: 
    CustomEnvironment*      env_ = nullptr;
    EnvironmentStatus       status_;
    StateTransitionInfo     transition_;

    std::vector<int>        markov_decision_chain_;  
    int                     current_state_;     
    int                     current_step_;
    bool                    session_initiated_ = false;       
};