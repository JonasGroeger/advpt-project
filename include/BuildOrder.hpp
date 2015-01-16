#pragma once

#include <vector>
#include <set>
#include <iostream>
#include <stdexcept>
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
    
    vector<BuildStep *>::iterator iterator;
    EntityType race;

    static map<EntityType, int> supply;

    // Check supply requirements
    bool isSupplyPossible();

public:
    vector<BuildStep *> buildSteps;
    BuildOrder(const char *file);
    BuildOrder(BuildOrder& other);

    BuildOrder(vector<BuildStep *> buildList);
    
    ~BuildOrder();

    static map<EntityType, vector<EntityType>> dependencies;

    // This will check for basic dependencies
    bool doSanityCheck();

    EntityType getRace();

    BuildStep *getNextStep();

    void advance();

    bool isDone();

    void addStepToBuildList(BuildStep* step);
    void removeLastStepFromBuildList();
    void clearBuildSteps();
    vector<BuildStep*> getBuildList();
};
