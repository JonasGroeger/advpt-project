#include "BuildOrder.h"

vector<BuildAction> BuildOrder::createBuildOrder(string target)
{
    vector<BuildAction> returnVec;
    returnVec.push_back(ConfigParser::Instance().getAction(target));
    getDependencies(ConfigParser::Instance().getAction(target).id, returnVec);
    reverse(returnVec.begin(), returnVec.end());
    return returnVec;
}

void BuildOrder::getDependencies(action_t id, vector<BuildAction>& outVector)
{
    auto dependencies = ConfigParser::Instance().getAction(id).dependencies;
    for(auto depPair : dependencies)
    {
        LOG_DEBUG("Dependency of [" << ConfigParser::Instance().getAction(id).name <<"] is " << depPair.second << "* [" << ConfigParser::Instance().getAction(depPair.first).name << "]");
        outVector.push_back(ConfigParser::Instance().getAction(depPair.first));
        getDependencies(ConfigParser::Instance().getAction(depPair.first).id, outVector);
    }
    return;
}
