#include "maze_application.h"


//i guess this is intended to work as a interface to the environment

MazeApplication::MazeApplication() : 
 environment_(std::make_unique<MazeEnvironment> (0))
{
    
}

void MazeApplication::run()
{  
    while (std::tolower(std::cin.get()) != 'q')
    {

    }
    
    exit(0);
}