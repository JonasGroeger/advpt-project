#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "BuildStep.hpp"
using namespace std;

class BuildOrder
{
private:
    vector<BuildStep*> buildSteps;
    vector<BuildStep*>::iterator iterator;
    
public:
    BuildOrder(char* file){
        ifstream infile(file);
        string line;
        while (getline(infile, line))
        {
            buildSteps.insert(buildSteps.end(), new BuildStep(line));
        }
        
        iterator = buildSteps.begin();
    }
    
    ~BuildOrder(){
    }
    
    BuildStep* getNextStep();
    void advance();
};
