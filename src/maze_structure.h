#pragma once 

#include <iostream>
#include <map>
#include <vector>
#include <memory>

class StateTransitionMap
{    
public :

    //returns possible transitions from each state

    std::pair<int,float> GetTransition(const int & state, const int & action);
    
    virtual void FillTransitionMap() = 0;
    virtual std::vector<int> GetLegalTransitions(const int & state) const = 0;

    protected : 
    int transition_map_[16*4]; //what if we want to keep this dynamic size?

};

class StateTransitionMapLevel1 : public StateTransitionMap
{
public :

    void FillTransitionMap() override;

    std::vector<int> GetLegalTransitions(const int & state) const;
};

class StateTransitionMapLevel2 : public StateTransitionMap
{
public :

    void FillTransitionMap() override;

    std::vector<int> GetLegalTransitions(const int & state) const;
};

class MazeStructure
{
public : 
    MazeStructure(const int & level);

    int GetTiles() const ;
    constexpr int GetOutputDimension() const  {
        return 4;
    }

    inline std::vector<int> GetLegalTransitions(const int & state) const{
        return state_transition_map_->GetLegalTransitions(state);
    }

    //returns possible transitions from each state
    std::pair<int,float> GetTransition(const int & state, const int & action);

private:
    
    int level_;
    std::unique_ptr<StateTransitionMap> state_transition_map_ = nullptr;
    int n_tiles_;
};