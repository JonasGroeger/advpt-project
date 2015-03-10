#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef struct BuildCost{
    int time;
    int gas;
    int minerals;
    int supply;
    vector<int> units;
} BuildCost;

typedef struct BuildResult{
    int gas;
    int minerals;
    int supply;
    vector<int> units;
} BuildResult;

typedef struct BuildAction{
    int id;
    string name;
    bool isWorker = false;
    bool isGasHarvester = false;
    BuildCost cost;
    vector<int> dependencies;
    vector<int> borrows;
    BuildResult result;
} BuildAction;
