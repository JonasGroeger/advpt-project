#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
#include <iostream>
#include <vector>
#include <algorithm>    // std::reverse

#include "Debug.h"

using namespace std;

class BuildOrder
{
public:
    BuildOrder(){};
    vector<BuildAction> createMinimalBuildOrder(string target);
    vector<BuildAction> getPossibleNextActions(const vector<BuildAction> actions);

private:
    int availableSupply = 0;
    map<action_t, int> availableUnits;
    vector<BuildAction> buildList;
    void getDependencies(action_t id, vector<BuildAction> &outVector);
    bool checkSupply(int costSupply, int currentSupply);
    bool checkDependencies(const vector<std::pair<action_t, int>> &dependencies);
    bool checkBorrows(const vector<std::pair<action_t, int>> &borrows);
};
