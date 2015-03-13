#include "State.h"

/*
 * This reserves the neccessary memory in the vectors
 * and intializes everything to 0
 */
State::State(const ConfigParser& cfg)
    : entities(cfg.getNumberOfActions(), 0),
      borrowed(cfg.getNumberOfActions(), 0),
      producing(cfg.getNumberOfActions(), 0)
{
}

bool State::isLegalAction(const BuildAction& act) const
{
    // Dependencies
    for (auto dep : act.dependencies)
    {
        action_t type = dep.first;
        int count = dep.second;

        // We also take currently produced entities in account
        if (entities[type] + producing[type] < count)
        {
            return false;
        }
    }

    // Borrows
    for (auto borrow : act.borrows)
    {
        action_t type = borrow.first;
        int count = borrow.second;

        if (entities[type] + producing[type] < count)
        {
            return false;
        }
    }

    // Costs
    /* TODO declare variables
    BuildCost& cost = act.cost;
    if (cost.minerals > 0 && !producingMinerals) return false;
    if (cost.gas > 0 && !producingGas) return false;
    if (cost.supply > (supply_max - supply_used) && !producingSupply) return false;
    */

    return true;
}

bool State::isSatisfied(const vector<std::pair<action_t, int>> entities, bool use_producing) const
{
    for (auto entity : entities)
    {
        action_t type = entity.first;
        int count = entitiy.second;

        if (entities[type] + producing[type] * use_producing < count)
        {
            return false;
        }
}

void State::advanceTime(time_t amount)
{
    time_t end_time = currentTime + amount;

    // Finish all actions that will end withing @amount
    // TODO read priority_queue docs
    // TODO is <= correct?
    while (activeActions.top().timeFinished <= currentTime + amount)
    {
        time_t time_delta = activeActions.top().timeFinished - currentTime;
        assert(time_delta >= 0);
        // First add Ressources
        increaseRessources(time_delta);
        // Increase time
        currentTime += time_delta;

        // Handle action results
        BuildAction& act = activeActions.top().action;
        activeActions.pop();

        if (act.isWorker) 
        {
            workersAll ++;

            // Always put workers into gas if possible
            if (workersGas < gasHarvesting)
            {
                workersGas++;
            }
            else
            {
                workersMinerals++;
            }
        }

        if (act.isGasHarvester)
        {
            gasHarvesting += 3;
        }

        BuildResult& res = act.result;

        minerals += res.minerals * RESS_FACTOR;
        gas += res.gas * RESS_FACTOR;

        supply_max += res.supply;

        for (auto unit : res.units)
        {
            entities[unit.first] += unit.second;
            producing[unit.first] -= unit.second;
        }
    }

    increaseRessources(currentTime - end_time);
    currentTime = end_time;
}

// TODO this one is tricky because we need to identify the actions upon which we have to wait
// Also for determining how long until ressources are available we have to take active actions into account that are producing workers which will increase ressource production
time_t State::isAdditionalTimeNeeded(const BuildCost& cost) const
{

    return 0;
}

void State::startAction(BuildAction& act)
{
    // TODO remember to add result to producing
    ActiveAction aa(currentTime + act.cost.time, act);
    activeActions.push(aa);

}

void State::increaseRessources(time_t t)
{
    assert(t >= 0);

    // *_PER_TIME_UNIT is already scaled by RESS_FACTOR
    minerals += t * MINERALS_PER_TIME_UNIT * workersMinerals;
    gas += t * GAS_PER_TIME_UNIT * workersGas;
}
