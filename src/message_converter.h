#pragma once
#include <map>
#include "utils.h"

//for future development, there might be different message types and structures which will need to be separated from utils class
class MessageConverter 
{
public : 
    static std::map<std::string, float> ConvertMessageToFloatMap(const std::string & msg ) {
        return Utils::ConvertStringToFloatMap(msg);
    }

};
