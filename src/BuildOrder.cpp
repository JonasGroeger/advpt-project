#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    reset();
    vector<action_t> deps;
    ConfigParser::Instance().setRaceForAction(target);
    BuildAction targetAction = ConfigParser::Instance().getAction(target);
    getDependencies(targetAction.id, deps);
    map<action_t, int> currUnits;

    int supply = 0;
    auto startUnits = ConfigParser::Instance().getStartConfig();
    for(auto startPair : startUnits)
    {
        LOG_DEBUG("ADDING START UNITS : [" << ConfigParser::Instance().getAction(startPair.first).name<<"] "<<startPair.second);
        currUnits[startPair.first] = startPair.second;
    }

    while(deps.size() > 0)
    {
        auto possibleActions = getPossibleNextActions(currUnits, deps);
        for(action_t action : possibleActions)
        {
            auto bAction = ConfigParser::Instance().getAction(action);
            while(!checkSupply(bAction.cost.supply, supply))
            {
                addOrIncrementUnit(currUnits, ConfigParser::Instance().getDefaulSupplyAction().id);
                supply = applySupply(supply, ConfigParser::Instance().getDefaulSupplyAction().id);
                buildList.push_back(ConfigParser::Instance().getDefaulSupplyAction().id);
            }
            addOrIncrementUnit(currUnits, action);
            supply = applySupply(supply, action);
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
    while(!checkSupply(targetAction.cost.supply, supply))
    {
        addOrIncrementUnit(currUnits, ConfigParser::Instance().getDefaulSupplyAction().id);
        supply = applySupply(supply, ConfigParser::Instance().getDefaulSupplyAction().id);
        buildList.push_back(ConfigParser::Instance().getDefaulSupplyAction().id);
    }
    buildList.push_back(targetAction.id);
}

void BuildOrder::getDependencies(action_t id, vector<action_t>& outVector)
{
    auto dependencies = ConfigParser::Instance().getAction(id).dependencies;
    for(auto depPair : dependencies)
    {
        LOG_DEBUG("Dependency of [" << ConfigParser::Instance().getAction(id).name <<"] is " << depPair.second << "* [" << ConfigParser::Instance().getAction(depPair.first).name << "]");
        if(availableUnits.count(ConfigParser::Instance().getAction(depPair.first).id) <= 0
                || (availableUnits[ConfigParser::Instance().getAction(depPair.first).id] < depPair.second))
        {
            LOG_DEBUG("Adding it to our vector!");
            outVector.push_back(ConfigParser::Instance().getAction(depPair.first).id);
        }
        addOrIncrementUnit(availableUnits, ConfigParser::Instance().getAction(depPair.first).id);
        getDependencies(ConfigParser::Instance().getAction(depPair.first).id, outVector);
    }
    return;
}


bool BuildOrder::insertActionIfPossible(action_t action, unsigned int position)
{
    //this should never hapen when calling method works correctly
    assert(position < buildList.size());

    // first get the "state" until pos-1 in our buildorder
    auto iter = buildList.begin();
    int supply = getSupply(position);
    map<action_t, int> currUnits;

    auto startUnits = ConfigParser::Instance().getStartConfig();
    for(auto startPair : startUnits)
    {
        LOG_DEBUG("ADDING START UNITS : [" << ConfigParser::Instance().getAction(startPair.first).name << "] "
                << startPair.second);
        currUnits[startPair.first] = startPair.second;
    }

    // Add the units until the insert position (excl.) in the list.
    for(int c = 0; c < position; c++)
    {
        addOrIncrementUnit(currUnits, (*iter));
        iter++;
    }

    // Check if the newly inserted action is possible at all
    if(!isActionPossible(currUnits, supply, action))
    {
        return false;
    }
    addOrIncrementUnit(currUnits, action);
    supply = applySupply(supply, action);

    for(;iter != buildList.end(); iter++)
    {
        if(!isActionPossible(currUnits, supply, (*iter))){
            return false;
        }
        addOrIncrementUnit(currUnits, (*iter));
        supply = applySupply(supply, (*iter));
    }
    buildList.insert(buildList.begin()+position, ConfigParser::Instance().getAction(action).id);
    return true;
}

bool BuildOrder::removeActionIfPossible(unsigned int position)
{
    assert(position < buildList.size());

    //first get the "state" until pos-1 in our buildorder
    auto iter = buildList.begin();
    int supply = getSupply(position);
    map<action_t, int> currUnits;

    auto startUnits = ConfigParser::Instance().getStartConfig();
    for(auto startPair : startUnits)
    {
        LOG_DEBUG("ADDING START UNITS : [" << ConfigParser::Instance().getAction(startPair.first).name<<"] "<<startPair.second);
        currUnits[startPair.first] = startPair.second;
    }

    for(int c = 0; c < position; c++)
    {
        addOrIncrementUnit(currUnits, (*iter));
        iter++;
    }
    //skip the action we want to remove
    iter++;

    for(;iter != buildList.end(); iter++)
    {
        if(!isActionPossible(currUnits, supply, (*iter))){
            return false;
        }
        addOrIncrementUnit(currUnits, (*iter));
        supply = applySupply(supply, (*iter));
    }
    buildList.erase(buildList.begin()+position);
    return true;
}

bool BuildOrder::isActionPossible(const map<action_t, int> &currentUnits, unsigned int currentSupply, action_t action)
{
    BuildAction bAction = ConfigParser::Instance().getAction(action);
    return checkSupply(bAction.cost.supply, currentSupply)
            && checkDependencies(bAction.dependencies, currentUnits)
            && checkBorrows(bAction.borrows, currentUnits)
            && checkMaxUnits(bAction.maxNumber, action, currentUnits);
}

vector<action_t> BuildOrder::getPossibleNextActions(const map<action_t, int> &currUnits, const vector<action_t> &actions)
{
    vector<action_t> resultVec;

    for(auto action : actions)
    {
        auto bAction = ConfigParser::Instance().getAction(action);
        // TODO
        if(checkDependencies(bAction.dependencies, currUnits)
                && checkBorrows(bAction.borrows, currUnits))
        {
            LOG_DEBUG("Action [" << bAction.name << "] is possible");
            resultVec.push_back(bAction.id);
        }
    }
    return resultVec;
}

bool BuildOrder::checkSupply(unsigned int costSupply, unsigned int currentSupply)
{
    return currentSupply >= costSupply;
}

void BuildOrder::reset()
{
    buildList.clear();
    availableUnits.clear();
}

int BuildOrder::getSupply(unsigned int index)
{
    int result = 0;
    int idx = 0;
    for(auto it = buildList.begin(); it != buildList.end() && idx < index; it++)
    {
        idx++;
        action_t action = (*it);
        result -= ConfigParser::Instance().getAction(action).cost.supply;
        result += ConfigParser::Instance().getAction(action).result.supply;
    }
    return result;
}

int BuildOrder::applySupply(unsigned int currSupply, action_t action)
{
    BuildAction bAction = ConfigParser::Instance().getAction(action);
    currSupply -= bAction.cost.supply;
    currSupply += bAction.result.supply;
    return currSupply;
}

void BuildOrder::addOrIncrementUnit(map<action_t, int> &unitMap, action_t unit)
{
    if(unitMap.count(unit)){
        unitMap[unit]++;
    } else
    {
        unitMap[unit] = 1;
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

bool BuildOrder::checkMaxUnits(unsigned int unitMax, action_t action, const map<action_t, int> &currentUnits)
{
    //this is our default case with an unlimited unit
    if(unitMax == 0) return true;
    //unit is not yet present in our current state
    if(currentUnits.count(action) == 0) return true;

    unsigned long unitCount = currentUnits.at(action);
    LOG_DEBUG("Checking Unit [" << ConfigParser::Instance().getAction(action).name <<"] with max of ["
            << ConfigParser::Instance().getAction(action).maxNumber << "] and currently available [" << unitCount << "]");
    return unitCount < unitMax;
}
