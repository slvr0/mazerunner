#include "maze_application.h"

MazeApplication::MazeApplication() : 
 environment_(std::make_unique<MazeEnvironment> (0))
{
    
}

void MazeApplication::run()
{  
    for(int i = 0 ; i < 100;++i)
    {
        environment_->Step(); // seg faulting ! happy new year     
    }
  

}