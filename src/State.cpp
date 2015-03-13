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
    if (!isSatisfied(act.dependencies, true))
    {
        LOG_DEBUG("dep failed");
        return false;
    }

    // Borrows
    if (!isSatisfied(act.borrows, true))
    {
        LOG_DEBUG("bor failed");
        return false;
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
        const BuildAction& act = activeActions.top().action;
        LOG_DEBUG("Handle action with id: " << act.id << " and finishTime: " << activeActions.top().timeFinished);
        activeActions.pop();

        const BuildResult& res = act.result;

        minerals += res.minerals * RESS_FACTOR;
        gas += res.gas * RESS_FACTOR;

        supply_max += res.supply;

        for (auto unit : res.units)
        {
            addUnit(unit.first, unit.second);
            producing[unit.first] -= unit.second;
        }
    }

    increaseRessources(currentTime - end_time);
    currentTime = end_time;
}

// TODO this one is tricky because we need to identify the actions upon which we have to wait
// Also for determining how long until ressources are available we have to take active actions into account that are producing workers which will increase ressource production
time_t State::isAdditionalTimeNeeded(const BuildAction& act) const
{
    assert(isLegalAction(act));
    // Dependencies
    if (!isSatisfied(act.dependencies, false)
     || !isSatisfied(act.borrows, false)
     || !hasEnoughSupply(act.cost.supply))
    {
        // TODO return time until next action is finished
        return 1;
    }

    ress_t minerals_needed = minerals - act.cost.minerals;
    ress_t gas_needed      = gas - act.cost.gas;

    if (minerals_needed < 0) minerals_needed = 0;
    if (gas_needed < 0) gas_needed = 0;

    // TODO division by zero
    ress_t minerals_time = minerals_needed / getMineralsPerTick();
    ress_t gas_time      = gas_needed / getGasPerTick();

    if (minerals_time > gas_time)
    {
        return minerals_time+1;
    }
    else
    {
        return gas_time+1;
    }
}

void State::startAction(const BuildAction& act)
{
    // TODO remember to add result to producing
    ActiveAction aa(currentTime + act.cost.time, act);
    activeActions.push(aa);
    
    LOG_DEBUG("inserted new action int queue with id: " << act.id << " finish time: " << aa.timeFinished);

    // TODO consume and borrow ressources
}

void State::addUnit(action_t type, int count)
{
    // TODO clean the worker detection up
    entities[unit] ++;
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

}

void State::increaseRessources(time_t t)
{
    assert(t >= 0);

    minerals += t * getMineralsPerTick();
    gas      += t * getGasPerTick();
}

bool State::isSatisfied(const vector<std::pair<action_t, int>>& entities, bool use_producing) const
{
    for (auto entity : entities)
    {
        action_t type = entity.first;
        int count = entity.second;

        if (this->entities[type] + producing[type] * use_producing < count)
        {
            return false;
        }
    }
    return true;
}

bool State::hasEnoughSupply(ress_t supply_needed) const
{
    return (supply_max - supply_used) >= supply_needed;
}

ress_t State::getMineralsPerTick() const
{
    // *_PER_TIME_UNIT is already scaled by RESS_FACTOR
    return MINERALS_PER_TIME_UNIT * workersMinerals;
}

ress_t State::getGasPerTick() const
{
    // *_PER_TIME_UNIT is already scaled by RESS_FACTOR
    return GAS_PER_TIME_UNIT * workersGas;
}
