#pragma once
#include <vector>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include "BuildStep.hpp"
using namespace std;

class BuildOrder
{
private:
    vector<BuildStep*> buildSteps;
    vector<BuildStep*>::iterator iterator;
    
public:
    BuildOrder(const char* file);
    ~BuildOrder();
    
    BuildStep* getNextStep();
    void advance();
};
