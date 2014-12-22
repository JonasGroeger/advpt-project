#pragma once

#include <vector>
#include <set>
#include <iostream>
//#include <exception>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

#include "BuildStep.hpp"
#include "EntityType.hpp"
#include "Entity.hpp"

using namespace std;

class BuildOrder
{
private:
    vector<BuildStep*> buildSteps;
    vector<BuildStep*>::iterator iterator;
    EntityType race;

    static map<EntityType, vector<EntityType>> dependencies;
    static map<EntityType, int> supply;

public:
    BuildOrder(const char* file);
    ~BuildOrder();
    
    // This will check for basic dependencies
    bool doSanityCheck();

    EntityType getRace();

    BuildStep* getNextStep();
    void advance();
    bool isDone();
};
