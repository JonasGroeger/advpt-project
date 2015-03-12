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
    vector<BuildAction> createBuildOrder(string target);

private:
    vector<BuildAction> buildList;
    void getDependencies(action_t id, vector<BuildAction> &outVector);
};
