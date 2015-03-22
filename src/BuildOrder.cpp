#include "BuildOrder.h"

// TODO remove state logic
void BuildOrder::createMinimalBuildOrder(string target)
{
    buildList.clear();
    ConfigParser::Instance().setRaceForAction(target);
    BuildAction targetAction = ConfigParser::Instance().getAction(target);

    State state = State(ConfigParser::Instance().getStartConfig());

    vector<action_t> dependencies = getDependencies(targetAction.id);
    dependencies.push_back(targetAction.id);

    while(dependencies.size() > 0)
    {
        auto possibleActions = getPossibleNextActions(dependencies, state);
        if(possibleActions.empty())
        {
            //seems like nothing from our dependencies is possible so add a supply here
            const auto& defaultSupply = ConfigParser::Instance().getDefaulSupplyAction();
            if(state.isLegalAction(defaultSupply))
            {
                startActionInState(defaultSupply.id, state);
                buildList.push_back(defaultSupply.id);
                continue;
            }
            else
            {
                throw std::logic_error("No supply possible");
            }
        }
        for(action_t action : possibleActions)
        {
            startActionInState(action, state);
            buildList.push_back(action);
            dependencies.erase(std::remove_if(dependencies.begin(),dependencies.end(),
                    [&action](action_t id)
                    {
                        return id == action;
                    }),dependencies.end());
        }
    }
}

unsigned int BuildOrder::getSize() const
{
    return buildList.size();
}

action_t BuildOrder::getAction(unsigned int position) const
{
    assert(position < buildList.size());
    return buildList[position];
}

//unsigned int BuildOrder::getFitness() const
ExecutionResult BuildOrder::execute(time_t maxTime) const
{
    ConfigParser &cfg = ConfigParser::Instance();
    State state(cfg.getStartConfig(), isForwardSim);

    for (unsigned int i = 0; i < buildList.size(); i++)
    {
        const auto &buildAction = cfg.getAction(buildList[i]);

        if (!state.isLegalAction(buildAction))
        {
            return {false, 0, i};
        }

        time_t t;
        while ((t = state.isAdditionalTimeNeeded(buildAction)) > 0)
        {
            LOG_DEBUG("STATE TIME NEEDED FOR ACTION [" + buildAction.name + "] IS [" << state.isAdditionalTimeNeeded(buildAction) << "]");
            state.advanceTime(t);

            if (maxTime > 0 && state.currentTime > maxTime)
            {
                return {false, state.currentTime, i-1};
            }
        }
        state.startAction(buildAction);
        if(isForwardSim)
        {
            state.printWorkers();
        }
        LOG_DEBUG("ACTION STARTED: [" << buildAction.name << "] ");
    }

    state.advanceTime(state.getTimeTillAllActionsAreFinished());

    if(isForwardSim)
    {
        state.printRessources();
    }

    return {true, state.currentTime, static_cast<vector<action_t>::difference_type> (buildList.size())-1};
}

unsigned int BuildOrder::getUnitCount(action_t target) const
{
    return getUnitCount(target, buildList.size()-1);
}

unsigned int BuildOrder::getUnitCount(action_t target, vector<action_t>::difference_type untilStep) const
{
    assert(untilStep >= -1 && untilStep < (vector<action_t>::difference_type)buildList.size());
    return count_if(buildList.begin(), buildList.begin()+untilStep+1,
            [&target](const action_t &entry)
            {
                return entry == target;
            });
}

vector<action_t> BuildOrder::getDependencies(action_t id) const
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

    buildList.insert(buildList.begin()+position, action);

    if (!execute().successfull)
    {
        buildList.erase(buildList.begin()+position);
        return false;
    }
    else
    {
        return true;
    }
}

bool BuildOrder::removeActionIfPossible(unsigned int position)
{
    assert(position < buildList.size());

    action_t saved = buildList[position];
    buildList.erase(buildList.begin()+position);

    if (!execute().successfull)
    {
        buildList.insert(buildList.begin()+position, saved);
        return false;
    }
    else
    {
        return true;
    }
}

bool BuildOrder::replaceActionIfPossible(action_t newAction, unsigned int position)
{
    assert(position < buildList.size());

    action_t saved = buildList[position];
    buildList.erase(buildList.begin()+position);
    buildList.insert(buildList.begin()+position, newAction);

    if (!execute().successfull)
    {
        buildList.erase(buildList.begin()+position);
        buildList.insert(buildList.begin()+position, saved);
        return false;
    }
    else
    {
        return true;
    }
}

void BuildOrder::setBuildList(const vector<action_t>& vec)
{
    buildList = vec;
}

const vector<action_t>& BuildOrder::getBuildList() const
{
    return buildList;
}

vector<action_t> BuildOrder::getPossibleNextActions(const vector<action_t> &actions, State& state) const
{
    vector<action_t> resultVec;

    for(auto action : actions)
    {
        const auto& bAction = ConfigParser::Instance().getAction(action);

        if(state.isLegalAction(bAction))
        {
            LOG_DEBUG("Action [" << bAction.name << "] is possible");
            resultVec.push_back(action);
        }
    }
    return resultVec;
}

void BuildOrder::startActionInState(action_t actionId, State& state) const
{
    const BuildAction& action = ConfigParser::Instance().getAction(actionId);
    while (state.isAdditionalTimeNeeded(action))
    {
        state.advanceTime(state.isAdditionalTimeNeeded(action));
    }
    state.startAction(action);
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
