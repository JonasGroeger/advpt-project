#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

using action_t = int;

typedef struct BuildCost
{
    int minerals;
    int gas;
    int time;
    int supply;
    vector<action_t> units;
} BuildCost;

typedef struct BuildResult
{
    int minerals;
    int gas;
    int supply;
    vector<action_t> units;
} BuildResult;

typedef struct BuildAction
{
    int id;
    string name;
    BuildCost cost;
    vector<action_t> dependencies;
    vector<action_t> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
} BuildAction;
