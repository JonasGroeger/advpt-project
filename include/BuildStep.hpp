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
    BuildStep(string value){
        name = value;
    }
    
    BuildStepType getType();
    Entity getWhich();
    std::string getName(){
        return name;
    }
};
