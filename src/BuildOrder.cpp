#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    reset();
    vector<action_t> deps;
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
                addOrIncrementUnit(&currUnits, ConfigParser::Instance().getDefaulSupplyAction().id);
                supply = applySupply(supply, ConfigParser::Instance().getDefaulSupplyAction().id);
                buildList.push_back(ConfigParser::Instance().getDefaulSupplyAction().id);
            }
            addOrIncrementUnit(&currUnits, action);
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
        addOrIncrementUnit(&currUnits, ConfigParser::Instance().getDefaulSupplyAction().id);
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
        addOrIncrementUnit(&availableUnits, ConfigParser::Instance().getAction(depPair.first).id);
        getDependencies(ConfigParser::Instance().getAction(depPair.first).id, outVector);
    }
    return;
}

bool BuildOrder::insertActionIfPossible(action_t action, int position)
{
    if(position < 0) return false;
    //assert(position > 0);
    if(buildList.size() < (unsigned int)position) return false;

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
    //assert(position > 0);
    if(buildList.size() < (unsigned int)position) return false;

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
    return checkSupply(bAction.cost.supply, currentSupply)
    && checkDependencies(bAction.dependencies, currentUnits)
    && checkBorrows(bAction.borrows, currentUnits);
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

bool BuildOrder::checkSupply(int costSupply, int currentSupply)
{
    return currentSupply >= costSupply;
}

void BuildOrder::reset(){
    buildList.clear();
    availableSupply = 0;
    availableUnits.clear();
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
