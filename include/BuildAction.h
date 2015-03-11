#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

using actiont = int;

typedef struct BuildCost
{
    int minerals;
    int gas;
    int time;
    int supply;
    vector<actiont> units;
} BuildCost;

typedef struct BuildResult
{
    int minerals;
    int gas;
    int supply;
    vector<actiont> units;
} BuildResult;

typedef struct BuildAction
{
    int id;
    string name;
    BuildCost cost;
    vector<actiont> dependencies;
    vector<actiont> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
} BuildAction;
