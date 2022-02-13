#pragma once
#include <vector>

//store structs / enums / other descriptors for loaded environment

class StateTransitionInfo 
{
    public:

    int state = 3;
    float reward = 0;
    int train = 0 ;
    int step = 0;
    std::vector<int> mask_transitions;
}; 

enum class EnvironmentStatus
{
    Finished = 0,
    Playing = 1
};