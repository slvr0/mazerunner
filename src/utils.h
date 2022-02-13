#pragma once
#include <vector>
#include <map>
#include <string.h>

class Utils
{
public : 
    //returns all indicies where the char is present in a string
    static std::vector<int> FindCharIdcs(std::string sample, char findIt); 

    static std::map<std::string, int> ConvertStringToIntMap(const std::string msg);
    static std::map<std::string, float> ConvertStringToFloatMap(const std::string msg);
        
};