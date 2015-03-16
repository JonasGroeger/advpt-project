#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

using action_t = int;

typedef struct BuildCost
{
    int minerals = 0;
    int gas = 0;
    int time = 0;
    int supply = 0;
    vector<std::pair<action_t, int>> units;
} BuildCost;

typedef struct BuildResult
{
    int minerals = 0;
    int gas = 0;
    int supply = 0;
    vector<std::pair<action_t, int>> units;
} BuildResult;

typedef struct BuildAction
{
    action_t id;
    string name;
    BuildCost cost;
    vector<std::pair<action_t, int>> dependencies;
    vector<std::pair<action_t, int>> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
    int maxNumber = 0;
} BuildAction;
