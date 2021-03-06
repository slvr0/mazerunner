#pragma once
#include <vector>

#include <map>
#include <string.h>

class Utils
{
public : 
    //returns all indicies where the char is present in a string
    static std::vector<int> FindCharIdcs(std::string sample, char findIt); 

    static std::map<std::string, int> ConvertJSONToIntMap(const std::string msg);
    static std::map<std::string, float> ConvertJSONToFloatMap(const std::string msg);

    template <typename T>
    static void PrintMap(std::map<std::string, T> __map);
        
};