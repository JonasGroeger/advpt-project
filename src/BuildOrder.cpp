#include "BuildOrder.h"

vector<BuildAction> BuildOrder::createBuildOrder(string target)
{
    auto bAction = ConfigParser::Instance().getAction(target);
    buildList.push_back(bAction);
    auto deps = bAction.dependencies;
    for(auto t : deps){

    }
}
