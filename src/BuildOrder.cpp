#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    // TODO this does not reset the rest of the BuildOrder State
    // TODO is this a problem?
    buildList.clear();
    vector<action_t> deps;
    getDependencies(ConfigParser::Instance().getAction(target).id, deps);
    map<action_t, int> currUnits;

    // TODO general approach for all races needed
    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("scv").id);
    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("command_center").id);

    while(deps.size() > 0)
    {
        LOG_DEBUG(deps.capacity());
        auto possibleActions = getPossibleNextActions(currUnits, deps);
        for(action_t action : possibleActions)
        {
            auto bAction = ConfigParser::Instance().getAction(action);
            addOrIncrementUnit(&currUnits, action);
            buildList.push_back(action);
            deps.erase(std::remove_if(deps.begin(),
                            deps.end(),
                            [&action](action_t id)
                            {
                                return id == action;
                            }),
                    deps.end());
        }
    }

    buildList.push_back(ConfigParser::Instance().getAction(target).id);

}

void BuildOrder::getDependencies(action_t id, vector<action_t>& outVector)
{
    auto dependencies = ConfigParser::Instance().getAction(id).dependencies;
    for(auto depPair : dependencies)
    {
        LOG_DEBUG("Dependency of [" << ConfigParser::Instance().getAction(id).name <<"] is " << depPair.second << "* [" << ConfigParser::Instance().getAction(depPair.first).name << "]");
        if(!(availableUnits.count(ConfigParser::Instance().getAction(depPair.first).id) > 0)
                || (availableUnits[ConfigParser::Instance().getAction(depPair.first).id] < depPair.second))
        {
            LOG_DEBUG("Adding it to our vector!");
            outVector.push_back(ConfigParser::Instance().getAction(depPair.first).id);
        }
        addOrIncrementUnit(&availableUnits, ConfigParser::Instance().getAction(depPair.first).id);
        getDependencies(ConfigParser::Instance().getAction(depPair.first).id, outVector);
    }
    return;
}

bool BuildOrder::insertActionIfPossible(action_t action, int position)
{
    if(position < 0) return false;
    if(buildList.size() < position) return false;

    //first get the "state" until pos-1 in our buildorder
    auto iter = buildList.begin();
    int supply = getSupply(position);
    map<action_t, int> currUnits;

    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("scv").id);
    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("command_center").id);

    for(int c = 0; c < position; c++)
    {
        addOrIncrementUnit(&currUnits, (*iter));
        iter++;
    }

    //check if action is possible
    if(!isActionPossible(currUnits, supply, action))
    {
        return false;
    }
    addOrIncrementUnit(&currUnits, action);
    supply = applySupply(supply, action);

    for(;iter != buildList.end(); iter++)
    {
        if(!isActionPossible(currUnits, supply, (*iter))){
            return false;
        }
        addOrIncrementUnit(&currUnits, (*iter));
        supply = applySupply(supply, (*iter));
    }
    buildList.insert(buildList.begin()+position, ConfigParser::Instance().getAction(action).id);
    return true;
}

bool BuildOrder::removeActionIfPossible(int position)
{
    if(position < 0) return false;
    if(buildList.size() < position) return false;

    //first get the "state" until pos-1 in our buildorder
    auto iter = buildList.begin();
    int supply = getSupply(position);
    map<action_t, int> currUnits;

    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("scv").id);
    addOrIncrementUnit(&currUnits, ConfigParser::Instance().getAction("command_center").id);

    for(int c = 0; c < position; c++)
    {
        addOrIncrementUnit(&currUnits, (*iter));
        iter++;
    }
    //skip the action we want to remove
    iter++;

    for(;iter != buildList.end(); iter++)
    {
        if(!isActionPossible(currUnits, supply, (*iter))){
            return false;
        }
        addOrIncrementUnit(&currUnits, (*iter));
        supply = applySupply(supply, (*iter));
    }
    buildList.erase(buildList.begin()+position);
    return true;
}

bool BuildOrder::isActionPossible(map<action_t, int> currentUnits, int currentSupply, action_t action)
{
    BuildAction bAction = ConfigParser::Instance().getAction(action);
    if(checkSupply(bAction.cost.supply, currentSupply)
        && checkDependencies(bAction.dependencies, currentUnits)
        && checkBorrows(bAction.borrows, currentUnits))
    {
        return true;
    }
    return false;
}

vector<action_t> BuildOrder::getPossibleNextActions(const map<action_t, int> &currUnits, const vector<action_t> &actions)
{
    vector<action_t> resultVec;
    //TODO REMOVE THIS AND GET OUT OF CONFIG assume that we have scv and command center
    int supply = 0;
    //availableUnits[ConfigParser::Instance().getAction("command_center").id] = 1;
    //availableUnits[ConfigParser::Instance().getAction("scv").id] = 5;

    for(auto action : actions)
    {
        auto bAction = ConfigParser::Instance().getAction(action);
        // TODO
        if(//checkSupply(action.cost.supply, supply)
                 checkDependencies(bAction.dependencies, currUnits)
                && checkBorrows(bAction.borrows, currUnits))
        {
            LOG_DEBUG("Action [" << bAction.name << "] is possible");
            resultVec.push_back(bAction.id);
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

int BuildOrder::getSupply(int pos)
{
    int result = 0;
    if(pos <= 0) return 0;
    int count = 0;
    for(auto iter = buildList.begin();
        iter != buildList.end() && count < pos;
        iter++)
    {
        count++;
        result -= ConfigParser::Instance().getAction((*iter)).cost.supply;
        result += ConfigParser::Instance().getAction((*iter)).result.supply;
    }
    return result;
}

int BuildOrder::applySupply(int currSupply, action_t action)
{
    BuildAction bAction = ConfigParser::Instance().getAction(action);
    currSupply -= bAction.cost.supply;
    currSupply += bAction.result.supply;
    return currSupply;
}

void BuildOrder::addOrIncrementUnit(map<action_t, int> *unitMap, action_t unit)
{
    if(unitMap->count(unit)){
        (*unitMap)[unit]++;
    } else
    {
        (*unitMap)[unit] = 1;
    }
}

bool BuildOrder::checkDependencies(const vector<std::pair<action_t, int>> &dependencies, const map<action_t, int> &currentUnits)
{
    for(auto dep : dependencies)
    {
        //if we dont have this dependency yet, or if we dont have enough instances of it, return false
        if(currentUnits.count(dep.first) == 0 || currentUnits.at(dep.first) < dep.second)
        {
            return false;
        }
    }
    return true;
}

bool BuildOrder::checkBorrows(const vector<std::pair<action_t, int>> &borrows, const map<action_t, int> &currentUnits)
{
    for(auto bor : borrows)
    {
        if(currentUnits.count(bor.first) == 0 || currentUnits.at(bor.first) < bor.second)
        {
            return false;
        }
    }
    return true;
}

ostream& operator<<(ostream &out, BuildOrder &obj)
{
    int c = 0;
    for(auto element : obj.buildList)
    {
        out << "[" << c << "] = [" << ConfigParser::Instance().getAction(element).name << "]" << std::endl;
        c++;
    }
    return out;
}
