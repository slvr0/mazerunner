#include "utils.h"

#include <iostream>

std::vector<int> Utils::FindCharIdcs(std::string sample, char findIt)
{
    std::vector<int> characterLocations;
    for(int i =0; i < sample.size(); i++)
        if(sample[i] == findIt) characterLocations.push_back(i);
    return characterLocations;
}

std::map<std::string, int> Utils::ConvertJSONToIntMap(const std::string msg)
{
    //improve algorithm by fixing nested dicts and accepting array formatting
    std::map<std::string, int> kvp;
    size_t start_curl = msg.find("{");
    size_t end_curl = msg.find("}");

    std::vector<int> vseps = Utils::FindCharIdcs(msg, ',');
    std::vector<int> seps = Utils::FindCharIdcs(msg, ':');    

    if(seps.size() == 0) return kvp;  
    
    //this should be its own algorithm, nothing to do with the taskleader 
    if(vseps.size() == 0) kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stoi(msg.substr(seps[0]+1, end_curl));  //first element in a single item list        
    else { 
    
        for(int i = 0 ; i < seps.size() ; ++i){
            if(i == 0) {            
                kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stoi(msg.substr(seps[0]+1, vseps[0])); //first element
            }
            else {            
                if(i == vseps.size()){ //last element                                   
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stoi(msg.substr(seps[i]+1, seps[i]+1 - end_curl - 1));
                }
                else{  //mid elements                
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stoi(msg.substr(seps[i] + 1, vseps[i] - seps[i] - 1));            
                }                  
            }

        }
    }
    return kvp;
}

std::map<std::string, float> Utils::ConvertJSONToFloatMap(const std::string msg)
{
    //improve algorithm by fixing nested dicts and accepting array formatting
    std::map<std::string, float> kvp;
    size_t start_curl = msg.find("{");
    size_t end_curl = msg.find("}");

    std::vector<int> vseps = Utils::FindCharIdcs(msg, ',');
    std::vector<int> seps = Utils::FindCharIdcs(msg, ':');    

    if(seps.size() == 0) return kvp;  
    
    //this should be its own algorithm, nothing to do with the taskleader 
    if(vseps.size() == 0) kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, end_curl));  //first element in a single item list        
    else { 
    
        for(int i = 0 ; i < seps.size() ; ++i){
            if(i == 0) {            
                kvp[msg.substr(start_curl+1, seps[0] - 1)] = std::stof(msg.substr(seps[0]+1, vseps[0])); //first element
            }
            else {            
                if(i == vseps.size()){ //last element                                   
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stof(msg.substr(seps[i]+1, seps[i]+1 - end_curl - 1));
                }
                else{  //mid elements                
                    kvp[msg.substr(vseps[i-1] + 1, seps[i] - vseps[i-1] - 1)] = 
                        std::stof(msg.substr(seps[i] + 1, vseps[i] - seps[i] - 1));            
                }                  
            }

        }
    }
    return kvp;
}

template <typename T>
void Utils::PrintMap(std::map<std::string, T> __map){
    std::cout << "\n";
    for(auto & [k, v] : __map){
        std::cout << k << ":" << std::to_string(v) << std::endl;
    }
    std::cout << "\n";
}
