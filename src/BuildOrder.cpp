#include "BuildOrder.h"

void BuildOrder::createMinimalBuildOrder(string target)
{
    reset();
    ConfigParser::Instance().setRaceForAction(target);
    BuildAction targetAction = ConfigParser::Instance().getAction(target);
    vector<action_t> dependencies = getDependencies(targetAction.id);

    state = State(ConfigParser::Instance().getStartConfig());
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

action_t BuildOrder::getAction(unsigned int position) const
{
    assert(position < buildList.size());
    return buildList[position];
}

unsigned int BuildOrder::getFitness()
{
    if(isDirty)
    {
        State state(ConfigParser::Instance().getStartConfig());

        for (auto action_id : buildList)
        {
            ConfigParser &cfg = ConfigParser::Instance();
            auto &buildAction = cfg.getAction(action_id);

            if (!state.isLegalAction(buildAction))
            {
                int c = 0;
                for (auto a : buildList)
                {
                    std::cout << "[" << c << "] - " << cfg.getAction(a).name << std::endl;
                    c++;
                }
                std::cout << state << std::endl;
                throw std::logic_error("Somethings wrong with this buildOrder ! " + buildAction.name + " is NEVER legal!");
            }

            time_t t;
            while ((t = state.isAdditionalTimeNeeded(buildAction)) > 0)
            {
                LOG_DEBUG("STATE TIME NEEDED FOR ACTION [" + buildAction.name + "] IS [" << state.isAdditionalTimeNeeded(buildAction) << "]");
                state.advanceTime(t);
                //LOG_DEBUG("ADVANCED BY [" << t << "] " << state);
            }
            state.startAction(buildAction);
            LOG_DEBUG("ACTION STARTED: [" << buildAction.name << "] ");
        }

        state.advanceTime(state.getTimeTillAllActionsAreFinished());

        fitness = state.currentTime;
        isDirty = false;
    }
    return fitness;
}

unsigned int BuildOrder::getUnitCount(action_t action, time_t maxTime)
{
    //TODO
    return 0;
}

vector<action_t> BuildOrder::getDependencies(action_t id)
{
    map<action_t, bool> availableUnits;
    vector<action_t> todo {id};
    vector<action_t> deps;

    while (!todo.empty())
    {
        action_t current = todo.back();
        todo.pop_back();

        for (auto depPair : ConfigParser::Instance().getAction(current).dependencies)
        {
            action_t type = depPair.first;
            if (!availableUnits[type])
            {
                todo.push_back(type);
                deps.push_back(type);
                availableUnits[type] = true;
            }
        }
    }

    return deps;
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

    if(applyBuildOrder(position, buildList.size()))
    {
        buildList.insert(buildList.begin()+position, ConfigParser::Instance().getAction(action).id);
        isDirty = true;
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
    if(applyBuildOrder(position+1, buildList.size()))
    {
        buildList.erase(buildList.begin()+position);
        isDirty = true;
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

    if(applyBuildOrder(position+1, buildList.size()))
    {
        buildList[position] = newAction;
        isDirty = true;
        return true;
    }
    return false;
}

vector<action_t> BuildOrder::getBuildList() const
{
    return buildList;
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
