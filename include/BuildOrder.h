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

    BuildOrder(string target)
    {
        createMinimalBuildOrder(target);
    };

    BuildOrder(const BuildOrder& other)
        :availableSupply(other.availableSupply)
        ,availableUnits(other.availableUnits)
        ,buildList(other.buildList)
    {};

    vector<action_t> getPossibleNextActions(const vector<BuildAction>& actions);
    bool insertActionIfPossible(action_t action, int position);
    bool removeActionIfPossible(int position);
    bool swapActionIfPossible(action_t old, action_t newAction, int position);

private:
    int availableSupply = 0;
    map<action_t, int> availableUnits;
    vector<BuildAction> buildList;

    void createMinimalBuildOrder(string target);
    void getDependencies(action_t id, vector<BuildAction> &outVector);
    bool checkSupply(int costSupply, int currentSupply);
    bool checkDependencies(const vector<std::pair<action_t, int>> &dependencies);
    bool checkBorrows(const vector<std::pair<action_t, int>> &borrows);
};
