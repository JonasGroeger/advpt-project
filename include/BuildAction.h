#pragma once

#include <string>
#include <vector>
#include <cassert>

#include "Debug.h"

using std::string;
using std::vector;

using action_t = int;
using energy_t = long double;

typedef struct BuildCost
{
    int minerals = 0;
    int gas = 0;
    int time = 0;
    int supply = 0;
    int larva = 0;
    energy_t energyAmount = 0;
    action_t energyFrom;
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
    bool isSpecial = false;
    bool hasEnergy = false;
    energy_t startEnergy = 0;
    energy_t maxEnergy = 0;
    BuildCost cost;
    vector<std::pair<action_t, int>> dependencies;
    vector<std::pair<action_t, int>> borrows;
    BuildResult result;

    bool isWorker = false;
    bool isGasHarvester = false;
    int maxNumber = 0;

    bool operator==(const BuildAction &rhs) const
    {
        if (id == rhs.id)
        {
            assert(name == rhs.name);
            return true;
        }
        else
        {
            return false;
        }
    }
} BuildAction;
