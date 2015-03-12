#include "BuildOrder.h"

BuildOrder::BuildOrder(ConfigParser parser)
{
    configParser = parser;
}

vector<BuildAction> BuildOrder::createBuildOrder(string target)
{
    buildList.clear;
    configParser.getAction(target);
}
