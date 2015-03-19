#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    reset();
    vector<action_t> dependencies;
    ConfigParser::Instance().setRaceForAction(target);
    BuildAction targetAction = ConfigParser::Instance().getAction(target);
    state = State(ConfigParser::Instance().getStartConfig());
    getDependencies(targetAction.id, dependencies);
    dependencies.push_back(targetAction.id);

    while(dependencies.size() > 0)
    {
        auto possibleActions = getPossibleNextActions(dependencies);
        if(possibleActions.size() == 0)
        {
            //seems like nothing from our dependencies is possible so add a supply here
            if(state.isLegalAction(ConfigParser::Instance().getDefaulSupplyAction()))
            {
                startActionInState(ConfigParser::Instance().getDefaulSupplyAction().id);
                buildList.push_back(ConfigParser::Instance().getDefaulSupplyAction().id);
            }
            continue;
        }
        for(action_t action : possibleActions)
        {
            auto bAction = ConfigParser::Instance().getAction(action);
            startActionInState(action);
            buildList.push_back(action);
            dependencies.erase(std::remove_if(dependencies.begin(),dependencies.end(),
                    [&action](action_t id)
                    {
                        return id == action;
                    }),dependencies.end());
        }
    }
}

unsigned int BuildOrder::getSize()
{
    return buildList.size();
}

unsigned int BuildOrder::getFitness()
{
    State state(ConfigParser::Instance().getStartConfig());

    for(auto action_id : buildList)
    {
        ConfigParser& cfg = ConfigParser::Instance();
        auto& buildAction = cfg.getAction(action_id);
        if(!state.isLegalAction(buildAction))
        {
            throw std::logic_error("Somethings wrong with this buildOrder ! " + buildAction.name + " is NEVER legal!");
        }
        
        time_t t;
        while((t = state.isAdditionalTimeNeeded(buildAction)) > 0)
        {
            LOG_DEBUG("STATE TIME NEEDED FOR ACTION ["+buildAction.name+"] IS ["<<state.isAdditionalTimeNeeded(buildAction) <<"]");
            state.advanceTime(t);
            //LOG_DEBUG("ADVANCED BY [" << t << "] " << state);
        }
        state.startAction(buildAction);
        LOG_DEBUG("ACTION STARTED: [" << buildAction.name << "] ");
    }

    state.advanceTime(state.getTimeTillAllActionsAreFinished());
    return state.currentTime;
}

unsigned int BuildOrder::getUnitCount(action_t action, time_t maxTime)
{
    //TODO
    return 0;
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

void BuildOrder::addOrIncrementUnit(map<action_t, int> &unitMap, action_t unit)
{
    if(unitMap.count(unit)){
        unitMap[unit]++;
    } else
    {
        unitMap[unit] = 1;
    }
}


bool BuildOrder::insertActionIfPossible(action_t action, unsigned int position)
{
    assert(position <= buildList.size());

    state = State(ConfigParser::Instance().getStartConfig());
    // first get the "state" until pos-1 in our buildorder
    applyBuildOrder(0, position);

    // Check if the newly inserted action is possible at all
    if(!state.isLegalAction(ConfigParser::Instance().getAction(action)))
    {
        return false;
    }
    startActionInState(ConfigParser::Instance().getAction(action).id);

    if(applyBuildOrder(position, buildList.size()-1))
    {
        buildList.insert(buildList.begin()+position, ConfigParser::Instance().getAction(action).id);
        return true;
    }
    return false;
}

bool BuildOrder::removeActionIfPossible(unsigned int position)
{
    assert(position < buildList.size());
    state = State(ConfigParser::Instance().getStartConfig());
    //first get the "state" until pos-1 in our buildorder
    applyBuildOrder(0, position);

    //skip the action we want to remove
    if(applyBuildOrder(position+1, buildList.size()-1))
    {
        buildList.erase(buildList.begin()+position);
        return true;
    }
    return false;
}

bool BuildOrder::replaceActionIfPossible(action_t newAction, unsigned int position)
{
    assert(position < buildList.size());

    state = State(ConfigParser::Instance().getStartConfig());
    //first get the "state" until pos-1 in our buildorder
    applyBuildOrder(0, position);

    // Check if the swapped action is possible
    if(!state.isLegalAction(ConfigParser::Instance().getAction(newAction)))
    {
        return false;
    }
    startActionInState(ConfigParser::Instance().getAction(newAction).id);

    if(applyBuildOrder(position+1, buildList.size()-1))
    {
        buildList[position] = newAction;
        return true;
    }
    return false;
}


vector<action_t> BuildOrder::getPossibleNextActions(const vector<action_t> &actions)
{
    vector<action_t> resultVec;

    for(auto action : actions)
    {
        auto bAction = ConfigParser::Instance().getAction(action);

        if(state.isLegalAction(bAction))
        {
            LOG_DEBUG("Action [" << bAction.name << "] is possible");
            resultVec.push_back(bAction.id);
        }
    }
    return resultVec;
}

void BuildOrder::startActionInState(const action_t &actionId)
{
    const BuildAction& action = ConfigParser::Instance().getAction(actionId);
    while (state.isAdditionalTimeNeeded(action))
    {
        state.advanceTime(state.isAdditionalTimeNeeded(action));
    }
    state.startAction(action);
}

bool BuildOrder::applyBuildOrder(unsigned int posStart, unsigned int posEnd)
{
    //first get the "state" until pos-1 in our buildorder
    auto iter = buildList.begin()+posStart;
    for (unsigned int index = posStart; index < posEnd; index++)
    {
        const BuildAction &action = ConfigParser::Instance().getAction(*iter);
        if(!state.isLegalAction(action))
        {
                cerr << state << endl;
                cerr << action.name << endl;
            return false;
        }
        startActionInState(action.id);
        iter++;
    }
    return true;
}

void BuildOrder::reset()
{
    buildList.clear();
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
