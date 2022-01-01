#pragma once 

#include <memory>

#include "environment.h"

class MazeApplication
{
public : 
    MazeApplication();
    void run();

private:
    std::unique_ptr<MazeEnvironment> environment_ = nullptr;


    

};