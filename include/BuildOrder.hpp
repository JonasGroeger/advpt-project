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
    EntityType race = NONE;

    static map<EntityType, int> supply;

    bool isVespenePossible();
    bool isDependenciesMet();
    bool isSupplyPossible();
    void checkIntegrity();

public:
    vector<BuildStep *> buildSteps;
    BuildOrder();
    BuildOrder(const char *file);
    BuildOrder(BuildOrder& other);

    BuildOrder(vector<BuildStep *> buildList);
    
    ~BuildOrder() = default;

    static map<EntityType, vector<EntityType>> dependencies;

    bool isPossible();

    EntityType getRace();

    BuildStep *getNextStep();

    void advance();

    bool isDone();

    void addBuildStep(string name);
    void addBuildStep(BuildStep *step);
    void removeLastStepFromBuildList();
    void clearBuildSteps();
    vector<BuildStep*> getBuildList();
    void print();
};
