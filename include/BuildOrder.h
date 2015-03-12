#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
#include <vector>
#include <algorithm>

class BuildOrder
{
public:
    BuildOrder(ConfigParser parser);
    vector<BuildAction> createBuildOrder(string target);

private:
    ConfigParser configParser;
    vector<BuildAction> buildList;
};
