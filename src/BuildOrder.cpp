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
        addOrIncrementUnit(&currUnits, (*iter).id);
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
        if(!isActionPossible(currUnits, supply, action)){
            return false;
        }
        addOrIncrementUnit(&currUnits, (*iter).id);
        supply = applySupply(supply, (*iter).id);
    }
    buildList.insert(buildList.begin()+position, ConfigParser::Instance().getAction(action));
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
                && checkDependencies(action.dependencies, availableUnits)
                && checkBorrows(action.borrows, availableUnits))
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
        result -= (*iter).cost.supply;
        result += (*iter).result.supply;
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
        out << "[" << c << "] = [" << element.name << "]" << std::endl;
        c++;
    }
    return out;
}
