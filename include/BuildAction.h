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
    vector<std::pair<action_t, int>> units;
} BuildCost;

typedef struct BuildResult
{
    int minerals;
    int gas;
    int supply;
    vector<std::pair<action_t, int>> units;
} BuildResult;

typedef struct BuildAction
{
    int id;
    string name;
    BuildCost cost;
    vector<std::pair<action_t, int>> dependencies;
    vector<std::pair<action_t, int>> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
} BuildAction;
