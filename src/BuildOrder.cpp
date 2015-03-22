#include "BuildOrder.h"

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
            const auto& bAction = ConfigParser::Instance().getAction(action);
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

unsigned int BuildOrder::getFitness() const
{
    ConfigParser &cfg = ConfigParser::Instance();
    State state(cfg.getStartConfig());

    for (auto action_id : buildList)
    {
        const auto &buildAction = cfg.getAction(action_id);

        if (!state.isLegalAction(buildAction))
        {
            for (unsigned int i = 0; i < buildList.size(); i++)
            {
                std::cerr << "[" << i << "] - " << cfg.getAction(buildList[i]).name << std::endl;
            }
            std::cerr << state << std::endl;
            throw std::logic_error("Somethings wrong with this buildOrder ! " + buildAction.name + " is NEVER legal!");
        }

        time_t t;
        while ((t = state.isAdditionalTimeNeeded(buildAction)) > 0)
        {
            LOG_DEBUG("STATE TIME NEEDED FOR ACTION [" + buildAction.name + "] IS [" << state.isAdditionalTimeNeeded(buildAction) << "]");
            state.advanceTime(t);
        }
        state.startAction(buildAction);
        LOG_DEBUG("ACTION STARTED: [" << buildAction.name << "] ");
    }

    state.advanceTime(state.getTimeTillAllActionsAreFinished());

    return state.currentTime;
}

unsigned int BuildOrder::getUnitCount(time_t maxTime) const
{
    int t = getFitness();
    if (t > maxTime)
    {
        return 0;
    }
    action_t target = targetUnit;
    return count_if(buildList.begin(), buildList.end(),
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

    const auto& act = ConfigParser::Instance().getAction(action);
    State state = State(ConfigParser::Instance().getStartConfig());

    // first get the "state" until pos-1 in our buildorder
    applyBuildOrderInState(0, position, state);

    // Check if the newly inserted action is possible at all
    if(!state.isLegalAction(act))
    {
        return false;
    }
    startActionInState(action, state);

    if(applyBuildOrderInState(position, buildList.size(), state))
    {
        buildList.insert(buildList.begin()+position, action);
        return true;
    }
    return false;
}

bool BuildOrder::removeActionIfPossible(unsigned int position)
{
    assert(position < buildList.size());

    State state = State(ConfigParser::Instance().getStartConfig());

    //first get the "state" until pos-1 in our buildorder
    applyBuildOrderInState(0, position, state);

    //skip the action we want to remove
    if(applyBuildOrderInState(position+1, buildList.size(), state))
    {
        buildList.erase(buildList.begin()+position);
        return true;
    }
    return false;
}

bool BuildOrder::replaceActionIfPossible(action_t newAction, unsigned int position)
{
    assert(position < buildList.size());

    const auto& act = ConfigParser::Instance().getAction(newAction);
    State state = State(ConfigParser::Instance().getStartConfig());

    //first get the "state" until pos-1 in our buildorder
    applyBuildOrderInState(0, position, state);

    // Check if the swapped action is possible
    if(!state.isLegalAction(act))
    {
        return false;
    }
    startActionInState(newAction, state);

    if(applyBuildOrderInState(position+1, buildList.size(), state))
    {
        buildList[position] = newAction;
        return true;
    }
    return false;
}

void BuildOrder::setBuildList(const vector<action_t>& vec)
{
        buildList = vec;
}

const vector<action_t>& BuildOrder::getBuildList() const
{
    return buildList;
}

// TODO
void BuildOrder::setTargetUnit(action_t target)
{
    targetUnit = target;
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

bool BuildOrder::applyBuildOrderInState(unsigned int posStart, unsigned int posEnd, State& state) const
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
        startActionInState(action.id, state);
        iter++;
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
