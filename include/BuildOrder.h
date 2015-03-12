#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BuildOrder
{
public:
    BuildOrder(){};
    vector<BuildAction> createBuildOrder(string target);

private:
    vector<BuildAction> buildList;
};
