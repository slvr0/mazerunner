#include "maze_structure.h"

#include <stdexcept>

MazeStructure::MazeStructure(const int & level) : 
level_(level),  state_transition_map_(std::make_unique<StateTransitionMapLevel2> ())
{
    {
        switch (level) {
        case 0 : {n_tiles_ = 16; break;}
        case 1 : {n_tiles_ = 16; break;}
        }
    }

    state_transition_map_->FillTransitionMap();
}

 int MazeStructure::GetTiles() const
 {
     return n_tiles_;
 }

std::vector<int> StateTransitionMapLevel1::GetLegalActions(const int & state) const
{   
    std::vector<int> action_mask;
       //go through the returns and see which gives a non negative one response
    if (transition_map_[state * 4 ] != -1) action_mask.emplace_back(0);
    if (transition_map_[state* 4 + 1] != -1) action_mask.emplace_back(1);
    if (transition_map_[state* 4 + 2] != -1) action_mask.emplace_back(2);
    if (transition_map_[state* 4 + 3] != -1) action_mask.emplace_back(3); 
    
    return action_mask; 
}


std::vector<int> StateTransitionMapLevel2::GetLegalActions(const int & state) const
{
    std::vector<int> action_mask;
       //go through the returns and see which gives a non negative one response
    if (transition_map_[state * 4 ] != -1) action_mask.emplace_back(0);
    if (transition_map_[state* 4 + 1] != -1) action_mask.emplace_back(1);
    if (transition_map_[state* 4 + 2] != -1) action_mask.emplace_back(2);
    if (transition_map_[state* 4 + 3] != -1) action_mask.emplace_back(3); 

    return action_mask; 
}

void StateTransitionMapLevel1::FillTransitionMap() 
{
    //** Explain? **/
    //first row represents the tile in the maze. the second row is the action (0 = R, 1 = D, 2 = L , 3 = U, 
    //and the resulting integer is the new square/state we end up in)
    std::cout << "creating action table (possible transitions) for map... \n";

    //smarter structure can be achieved!
    transition_map_[0] = 1;
    transition_map_[1] = -1;
    transition_map_[2] = -1;
    transition_map_[3] = -1;

    transition_map_[4] = 2;
    transition_map_[5] = -1;
    transition_map_[6] = 0; 
    transition_map_[7] = -1;

    transition_map_[8] = 3;
    transition_map_[9] = -1;
    transition_map_[10] = 1; 
    transition_map_[11] = -1; 

    transition_map_[12] = -1;
    transition_map_[13] = -1; 
    transition_map_[14] = 2; 
    transition_map_[15] = 7; 
     
    transition_map_[16] = 5;
    transition_map_[17] = -1; 
    transition_map_[18] = -1;
    transition_map_[19] = 8;
    
    transition_map_[20] = -1; 
    transition_map_[21] = -1; 
    transition_map_[22] = 4; 
    transition_map_[23] = 9;

    transition_map_[24] = 7;
    transition_map_[25] = -1; 
    transition_map_[26] = -1;
    transition_map_[27] = -1;

    transition_map_[28] = -1;    
    transition_map_[29] = 3; 
    transition_map_[30] = 6; 
    transition_map_[31] = 11;     
   
    transition_map_[32] = -1;
    transition_map_[33] = 4;      
    transition_map_[34] = -1; 
    transition_map_[35] = 12;
    
    transition_map_[36] = -1;
    transition_map_[37] = 5; 
    transition_map_[38] = -1;
    transition_map_[39] = 13;

    transition_map_[40] = 11; 
    transition_map_[41] = -1;
    transition_map_[42] = -1;
    transition_map_[43] = 14;
    
    transition_map_[44] = -1;
    transition_map_[45] = 7; 
    transition_map_[46] = 10; 
    transition_map_[47] = 15; 

    transition_map_[48] = -1;
    transition_map_[49] = -1; 
    transition_map_[50] = -1; 
    transition_map_[51] = -1; 
   
    transition_map_[52] = 14;
    transition_map_[53] = 9; 
    transition_map_[54] = -1; 
    transition_map_[55] = -1;     
    
    transition_map_[56] = 15; 
    transition_map_[57] = 10; 
    transition_map_[58] = 13; 
    transition_map_[59] = -1; 

    transition_map_[60] = -1; 
    transition_map_[61] = 11;
    transition_map_[62] = 14;
    transition_map_[63] = -1; 
    
}

void StateTransitionMapLevel2::FillTransitionMap() 
{
    //** Explain? **/
    //first row represents the tile in the maze. the second row is the action (0 = R, 1 = D, 2 = L , 3 = U, 
    //and the resulting integer is the new square/state we end up in)
    std::cout << "creating action table (possible transitions) for map... \n";

    //smarter structure can be achieved!
    transition_map_[0] = 1;
    transition_map_[1] = -1;
    transition_map_[2] = -1;
    transition_map_[3] = 4;

    transition_map_[4] = 2;
    transition_map_[5] = -1;
    transition_map_[6] = 0; 
    transition_map_[7] = 5;

    transition_map_[8] = 3;
    transition_map_[9] = -1;
    transition_map_[10] = 1; 
    transition_map_[11] = 6; 

    transition_map_[12] = -1;
    transition_map_[13] = -1; 
    transition_map_[14] = 2; 
    transition_map_[15] = 7; 
     
    transition_map_[16] = 5;
    transition_map_[17] = 0; 
    transition_map_[18] = -1;
    transition_map_[19] = 8;
    
    transition_map_[20] = 6; 
    transition_map_[21] = 1; 
    transition_map_[22] = 4; 
    transition_map_[23] = 9;

    transition_map_[24] = 7;
    transition_map_[25] = 2; 
    transition_map_[26] = 5;
    transition_map_[27] = 10;

    transition_map_[28] = -1;    
    transition_map_[29] = 3; 
    transition_map_[30] = 6; 
    transition_map_[31] = 11;     
   
    transition_map_[32] = 9;
    transition_map_[33] = 4;      
    transition_map_[34] = -1; 
    transition_map_[35] = 12;
    
    transition_map_[36] = 10;
    transition_map_[37] = 5; 
    transition_map_[38] = 8;
    transition_map_[39] = 13;

    transition_map_[40] = 11; 
    transition_map_[41] = 6;
    transition_map_[42] = 9;
    transition_map_[43] = 14;
    
    transition_map_[44] = -1;
    transition_map_[45] = 7; 
    transition_map_[46] = 10; 
    transition_map_[47] = 15; 

    transition_map_[48] = -1;
    transition_map_[49] = -1; 
    transition_map_[50] = -1; 
    transition_map_[51] = -1; 
   
    transition_map_[52] = 14;
    transition_map_[53] = 9; 
    transition_map_[54] = 12; 
    transition_map_[55] = -1;     
    
    transition_map_[56] = 15; 
    transition_map_[57] = 10; 
    transition_map_[58] = 13; 
    transition_map_[59] = -1; 

    transition_map_[60] = -1; 
    transition_map_[61] = 11;
    transition_map_[62] = 14;
    transition_map_[63] = -1;     
}

//return is new state, with reward(its simple, 0 for all space except goal)

std::pair<int, float> StateTransitionMap::GetTransition(const int & state, const int & action)
{
    std::pair<int,float> state_action_pair;

    state_action_pair.first = transition_map_[state * 4 + action];

    if(state_action_pair.first == -1)  //for now lets give it a negative reward for stepping to invalid squares and just keep the same state.
    {
        state_action_pair.first = state;
        state_action_pair.second = 0.0f;
    }
    else
    {
        state_action_pair.second = state_action_pair.first == 12 ? 1.0 : 0.0f;
    }

    return state_action_pair;
}




std::pair<int,float> MazeStructure::GetTransition(const int & state, const int & action)
{
    if(state >= n_tiles_) throw std::invalid_argument( "State does not exist in Maze... \n" );
    
    return state_transition_map_->GetTransition(state, action);
}
