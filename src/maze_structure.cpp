#include "maze_structure.h"

#include <stdexcept>

MazeStructure::MazeStructure(const int & level) : 
level_(level),  state_transition_map_(std::make_unique<StateTransitionMapLevel1> ())
{
    {
        switch (level) 
        case 0 : n_tiles_ = 16;
    }

    state_transition_map_->FillTransitionMap();
}

 int MazeStructure::GetTiles() const
 {
     return n_tiles_;
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

    
    transition_map_[48+4] = 14;
    transition_map_[49+4] = 9; 
    transition_map_[50+4] = -1; 
    transition_map_[51+4] = -1;     
    
    transition_map_[52+4] = 15; 
    transition_map_[53+4] = 10; 
    transition_map_[54+4] = 13; 
    transition_map_[55+4] = -1; 

    transition_map_[56+4] = -1; 
    transition_map_[57+4] = 11;
    transition_map_[58+4] = 15;
    transition_map_[59+4] = -1; 
    /*   

    transition_map_[0][0] = 1;
    transition_map_[0][2] = -1;
    transition_map_[0][4] = -1;
    transition_map_[0][8] = -1;

    transition_map_[1][0] = 2;
    transition_map_[1][2] = -1;
    transition_map_[1][4] = 0; 
    transition_map_[1][8] = -1;

    transition_map_[2][0] = 3;
    transition_map_[2][2] = -1;
    transition_map_[2][4] = 1; 
    transition_map_[2][8] = -1; 

    transition_map_[3][0] = -1;
    transition_map_[3][2] = -1; 
    transition_map_[3][4] = 2; 
    transition_map_[3][8] = 7; 
     
    transition_map_[4][0] = 5;
    transition_map_[4][2] = -1; 
    transition_map_[4][4] = -1;
    transition_map_[4][8] = 8;
    
    transition_map_[5][0] = -1; 
    transition_map_[5][2] = -1; 
    transition_map_[5][4] = 4; 
    transition_map_[5][8] = 9;

    transition_map_[6][0] = 7;
    transition_map_[6][2] = -1; 
    transition_map_[6][4] = -1;
    transition_map_[6][8] = -1;

    transition_map_[7][0] = -1;    
    transition_map_[7][2] = 3; 
    transition_map_[7][4] = 6; 
    transition_map_[7][8] = 11;     
   
    transition_map_[8][0] = -1;
    transition_map_[8][2] = 4;      
    transition_map_[8][4] = -1; 
    transition_map_[8][8] = 12;
    
    transition_map_[9][0] = -1;
    transition_map_[9][2] = 5; 
    transition_map_[9][4] = -1;
    transition_map_[9][8] = 13;

    transition_map_[10][0] = 11; 
    transition_map_[10][2] = -1;
    transition_map_[10][4] = -1;
    transition_map_[10][8] = 14;
    
    transition_map_[11][0] = -1;
    transition_map_[11][2] = 7; 
    transition_map_[11][4] = 10; 
    transition_map_[11][8] = 16;     
    
    transition_map_[13][0] = 14;
    transition_map_[13][2] = 9; 
    transition_map_[13][4] = -1; 
    transition_map_[13][8] = -1;     
    
    transition_map_[14][0] = 15; 
    transition_map_[14][2] = 10; 
    transition_map_[14][4] = 13; 
    transition_map_[14][8] = -1; 

    transition_map_[15][0] = -1; 
    transition_map_[15][2] = 11;
    transition_map_[15][4] = 15;
    transition_map_[15][8] = -1;

    */  
  
}

//return is new state, with reward(its simple, 0 for all space except goal)
std::pair<int, int> StateTransitionMap::GetTransition(const int & state, const int & action)
{
    std::pair<int,int> state_action_pair;

    state_action_pair.first = transition_map_[state*4 + action];
    state_action_pair.second = state_action_pair.first == 12 ? 0 : 1;

    return state_action_pair;
}


std::pair<int,int> MazeStructure::GetTransition(const int & state, const int & action)
{
    if(state >= n_tiles_) throw std::invalid_argument( "State does not exist in Maze... \n" );
    
    return state_transition_map_->GetTransition(state, action);
}
