#pragma once
#include "Entity.hpp"
#include "BuildStepType.hpp"

using namespace std;


class BuildStep
{
private:
    BuildStepType type;
    Entity which;
    string name;
public:
    //The name argument is just for testing
    //TODO
    BuildStep(string value){
        name = value;
    }
    
    BuildStepType getType();
    Entity getWhich();
    
    //Just for testing
    std::string getName(){
        return name;
    }
};
