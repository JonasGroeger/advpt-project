#include "State.h"

bool State::isLegalAction(const BuildAction& act)
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
time_t State::isAdditionalTimeNeeded(const BuildAction& act)
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

    ress_t minerals_needed = act.cost.minerals - minerals;
    ress_t gas_needed      = act.cost.gas - gas;

    if (minerals_needed < 0) minerals_needed = 0;
    if (gas_needed < 0) gas_needed = 0;

    assert(minerals_needed == 0 || getMineralsPerTick() != 0);
    assert(gas_needed == 0 || getGasPerTick() != 0);
    
    ress_t minerals_time = minerals_needed / getMineralsPerTick();
    if (minerals_needed == 0) minerals_time = 0;
    ress_t gas_time      = gas_needed / getGasPerTick();
    if (gas_needed == 0) gas_time = 0;

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
    // Checking dependencies
    assert(isSatisfied(act.dependencies, false));

    ActiveAction aa(currentTime + act.cost.time, act);
    activeActions.push(aa);
    
    LOG_DEBUG("inserted new action int queue with id: " << act.id << " finish time: " << aa.timeFinished);

    const BuildCost& cost = act.cost;

    // Consume minerals, gas and supply
    assert(minerals >= cost.minerals);
    minerals -= cost.minerals;
    assert(gas >= cost.gas);
    gas -= cost.gas;
    supply_used += cost.supply;
    assert(supply_used <= supply_max);


    // Remove the unit cost
    for (std::pair<action_t, int> unit : cost.units)
    {
        assert(entities[unit.first] >= unit.second);
        entities[unit.first] -= unit.second;
    }

    // Borrow some units
    for (std::pair<action_t, int> borrow : act.borrows)
    {
        borrowed[borrow.first] += borrow.second;
        assert(borrowed[borrow.first] <= entities[borrow.first]);
    }

    // We mark the results as being produced
    for (std::pair<action_t, int> result : act.result.units)
    {
            producing[result.first] += result.second;
    }
}

void State::addUnit(action_t type, int count)
{
    entities[type] += count;

    // This ain't pretty but it works
    const BuildAction& act = ConfigParser::Instance().getAction(type);
    if (act.isGasHarvester)
    {
        gasHarvesting += 3;
    }

    if (act.isWorker) 
    {
        workersAll += count;
    }

    if (act.isWorker || act.isGasHarvester)
    {
        reallocateWorkers();
    }
}

void State::increaseRessources(time_t t)
{
    assert(t >= 0);

    minerals += t * getMineralsPerTick();
    gas      += t * getGasPerTick();
}

bool State::isSatisfied(const vector<std::pair<action_t, int>>& constrains, bool use_producing)
{
    for (auto entity : constrains)
    {
        action_t type = entity.first;
        int count = entity.second;

        if (entities[type] + producing[type] * use_producing < count)
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

void State::reallocateWorkers()
{
    if (workersAll == 0)
    {
        return;
    }

    int workersRemaining = workersAll;
    
    // Always have at least one worker mine minerals
    workersMinerals = 1;
    workersRemaining --;

    if (gasHarvesting >= workersRemaining)
    {
        workersGas = workersRemaining;
        workersRemaining = 0;
    }
    else
    {
        workersGas = gasHarvesting;
        workersRemaining -= gasHarvesting;
    }

    workersMinerals += workersRemaining;
}
