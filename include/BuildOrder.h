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


    vector<action_t> getPossibleNextActions(const map<action_t, int> &currUnits, const vector<action_t> &actions);
    bool insertActionIfPossible(action_t action, int position);
    bool removeActionIfPossible(int position);
    bool swapActionIfPossible(action_t old, action_t newAction, int position);

    friend ostream& operator<< (ostream &out, BuildOrder &obj);

private:
    int availableSupply = 0;
    map<action_t, int> availableUnits;
    vector<action_t> buildList;

    bool isActionPossible(map<action_t, int> currentUnits, int currentSupply, action_t action);
    /*
     * Removes all build steps and creates a fresh BuildOrder
     * that will create the unit represented by @target
     */
    void createMinimalBuildOrder(string target);
    void getDependencies(action_t id, vector<action_t> &outVector);

    //get the resulting supply of this buildorder from step 0 to step pos-1
    int getSupply(int pos);
    int applySupply(int currSupply, action_t action);
    void addOrIncrementUnit(map<action_t, int> *unitMap, action_t unit);
    bool checkSupply(int costSupply, int currentSupply);
    bool checkDependencies(const vector<std::pair<action_t, int>> &dependencies, const map<action_t, int> &currentUnits);
    bool checkBorrows(const vector<std::pair<action_t, int>> &borrows, const map<action_t, int> &currentUnits);
};
