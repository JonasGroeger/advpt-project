#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

typedef struct BuildCost
{
    int minerals;
    int gas;
    int time;
    int supply;
    vector<int> units;
} BuildCost;

typedef struct BuildResult
{
    int minerals;
    int gas;
    int supply;
    vector<int> units;
} BuildResult;

typedef struct BuildAction
{
    int id;
    string name;
    BuildCost cost;
    vector<int> dependencies;
    vector<int> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
} BuildAction;
