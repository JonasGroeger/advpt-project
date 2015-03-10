#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef struct BuildResult{
    int gas;
    int minerals;
    int supply;
    vector<int> units;
} BuildResult;

typedef struct BuildAction{
    int id;
    string name;
    int neededMinerals;
    int neededGas;
    int neededTime;
    //could also be positive!
    int neededSupply;
    vector<int> dependencies;
    vector<int> borrows;
    BuildResult result;
} BuildAction;
