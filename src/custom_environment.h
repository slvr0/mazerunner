#pragma once
#include <vector>

#include "env_model.h"

class CustomEnvironment 
{

public : 
    CustomEnvironment(int changethis)  {
        
    }
    ~CustomEnvironment() = default;

    constexpr int GetOutputDimension() const{
        return output_dimensions_;
    }
        
    inline std::vector<int> GetLegalActions(StateTransitionInfo state_transition_info){
        //implement me
        return std::vector<int> {1,2,3};
    }

    inline float GetTransitionReward(StateTransitionInfo state_transition_info, int action) {
        //implement me
        return 0.f;
    }
    
    //new state, reward
    inline std::pair<int, float> GetTransition(StateTransitionInfo state_transition_info, int action) {
        //implement me
        return std::pair<int, float> {0, 0.f};
    }  

    private:
    int output_dimensions_;

};