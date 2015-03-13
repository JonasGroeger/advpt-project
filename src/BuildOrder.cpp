#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    buildList.clear();
    buildList.push_back(ConfigParser::Instance().getAction(target));
    getDependencies(ConfigParser::Instance().getAction(target).id, buildList);
    reverse(buildList.begin(), buildList.end());
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

vector<action_t> BuildOrder::getPossibleNextActions(const vector<BuildAction>& actions)
{
    vector<action_t> resultVec;
    //TODO REMOVE THIS AND GET OUT OF CONFIG assume that we have scv and command center
    int supply = 0;
    availableUnits[ConfigParser::Instance().getAction("command_center").id] = 1;
    availableUnits[ConfigParser::Instance().getAction("scv").id] = 5;

    for(auto action : actions)
    {
        if(checkSupply(action.cost.supply, supply)
                && checkDependencies(action.dependencies)
                && checkBorrows(action.borrows))
        {
            LOG_DEBUG("Action [" << action.name << "] is possible");
            resultVec.push_back(action.id);
        }
    }
    return resultVec;
}

bool BuildOrder::checkSupply(int costSupply, int currentSupply)
{
    if(currentSupply < costSupply)
    {
        return false;
    }
    return true;
}

bool BuildOrder::checkDependencies(const vector<std::pair<action_t, int>> &dependencies)
{
    for(auto dep : dependencies)
    {
        //if we dont have this dependency yet, or if we dont have enough instances of it, return false
        if(availableUnits.count(dep.first) == 0 || availableUnits[dep.first] < dep.second)
        {
            return false;
        }
    }
    return true;
}

bool BuildOrder::checkBorrows(const vector<std::pair<action_t, int>> &borrows)
{
    for(auto bor : borrows)
    {
        if(availableUnits.count(bor.first) == 0 || availableUnits[bor.first] < bor.second)
        {
            return false;
        }
    }
    return true;
}
