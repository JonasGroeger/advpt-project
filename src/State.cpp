#include "State.h"

State::State(const map<action_t, int> &startConfig)
{
    for (auto unit : startConfig)
    {
        action_t type = unit.first;
        int count = unit.second;

        const BuildAction& act = ConfigParser::Instance().getAction(type);

        for (int i = 0; i < count; i++)
        {
            addActionResult(act.result, false);
            supply_used += act.cost.supply;
        }
    }
    // TODO this is a hack
    future_supply_max = supply_max;
    assert(supply_max >= supply_used);

    this->minerals = 50 * RESS_FACTOR;
}

bool State::operator==(const State &rhs) const
{
    if (currentTime != rhs.currentTime
     || minerals != rhs.minerals
     || gas != rhs.gas
     || supply_used != rhs.supply_used
     || supply_max != rhs.supply_max
     || future_supply_max != rhs.future_supply_max
     || entities != rhs.entities
     || borrowed != rhs.borrowed
     || producing != rhs.producing)
    {
        return false;
    }
    return true;
}

bool State::operator!=(const State &rhs) const
{
    return !(*this == rhs);
}

bool State::isLegalAction(const BuildAction& act)
{
    // Dependencies
    if (!isSatisfied(act.dependencies, true))
    {
        LOG_DEBUG("dependencies not met");
        return false;
    }

    // TODO handle actions that cost and borrow untis of the same type
    // Costs
    const BuildCost& cost = act.cost;

    for (auto p : cost.units)
    {
            action_t type = p.first;
            int count = p.second;

            if (entities[type] + producing[type] < count)
            {
                    return false;
            }
    }

    // Borrows
    if (act.borrows.size() != 0)
    {
        bool fulfilled = false;
        for (auto entity : act.borrows)
        {
            action_t type = entity.first;
            int count = entity.second;

            if (entities[type] + producing[type] >= count)
            {
                fulfilled = true;
                break;
            }
        }
        if (!fulfilled)
        {
            LOG_DEBUG("borrows not met for " << act.name);
            return false;
        }
    }

    // Energy
    if (cost.energyAmount != 0)
    {
            if (entities[cost.energyFrom] + producing[cost.energyFrom] < 1)
            {
                    return false;
            }
    }

    if (supply_used + act.cost.supply > future_supply_max)
    {
            LOG_DEBUG("supply check failed: " << act.cost.supply << " > " << future_supply_max);
            return false;
    }

    return true;
}

void State::advanceTime(time_t amount)
{
    LOG_DEBUG("Advance time from [" << currentTime << "] by [" << amount << "]");
    time_t end_time = currentTime + amount;

    // Finish all actions that will end withing @amount
    while (!activeActions.empty() && activeActions.top().timeFinished <= end_time)
    {
        ActiveAction aa = activeActions.top();
        activeActions.pop();

        time_t time_delta = aa.timeFinished - currentTime;

        assert(time_delta >= 0);
        // First add Ressources
        increaseRessources(time_delta);
        // Increase time
        currentTime += time_delta;

        // Handle action results
        const BuildAction* act = aa.action;
        LOG_DEBUG("Handle action [" << act->name << "] id: " << act->id << " and finishTime: " << aa.timeFinished);

        addActionResult(act->result);

        // Unborrow units
        if (aa.borrowedAction != -1)
        {
                borrowed[aa.borrowedAction] --;
                assert(borrowed[aa.borrowedAction] >= 0);
        }

        // Register energy
        if (act->hasEnergy)
        {
                energyManager.registerNew(act->id, act->startEnergy, act->maxEnergy);
        }

        // Handle special abilities
        if (act->isSpecial)
        {
                // TODO handle stuff here
        }
    }
    increaseRessources(end_time-currentTime);
    currentTime = end_time;
}

time_t State::isAdditionalTimeNeeded(const BuildAction& act)
{
    assert(isLegalAction(act));
    // Dependencies and supply
    if (!isSatisfied(act.dependencies, false)
     || !hasEnoughSupply(act.cost.supply))
    {
        return getTimeTillNextActionIsFinished();
    }

    // Borrows
    bool borrowFound = act.borrows.empty();
    for (auto entity : act.borrows)
    {
        action_t type = entity.first;
        int count = entity.second;

        if (entities[type] - borrowed[type] >= count)
        {
            borrowFound = true;
            break;
        }
    }
    if (!borrowFound)
    {
        return getTimeTillNextActionIsFinished();
    }

    // costs
    for (auto entity : act.cost.units)
    {
        action_t type = entity.first;
        int count = entity.second;

        if (entities[type] - borrowed[type] < count)
        {
            return getTimeTillNextActionIsFinished();
        }
    }

    // Energy
    if (act.cost.energyAmount != 0)
    {
            time_t t = energyManager.timeUntilEnergyIsAvailable(act.cost.energyFrom, act.cost.energyAmount);
            if (t > 0)
            {
                    return std::max(t, getTimeTillNextActionIsFinished());
            }
    }

    ress_t minerals_needed = act.cost.minerals * RESS_FACTOR - minerals;
    ress_t gas_needed      = act.cost.gas * RESS_FACTOR - gas;

    // Maybe we have to wait until workers are produced
    if (minerals_needed != 0 && getMineralsPerTick() == 0)
    {
        return getTimeTillNextActionIsFinished();
    }
    if (gas_needed != 0 && getGasPerTick() == 0)
    {
        return getTimeTillNextActionIsFinished();
    }

    if (minerals_needed < 0) minerals_needed = 0;
    if (gas_needed < 0) gas_needed = 0;

    assert(minerals_needed == 0 || getMineralsPerTick() != 0);
    assert(gas_needed == 0 || getGasPerTick() != 0);
    
    ress_t minerals_time = std::ceil(double(minerals_needed) / double(getMineralsPerTick()));
    ress_t gas_time      = std::ceil(double(gas_needed) / double(getGasPerTick()));

    if (minerals_needed == 0) minerals_time = 0;
    if (gas_needed == 0) gas_time = 0;

    if (minerals_time > gas_time)
    {
        return minerals_time;
    }
    else
    {
        return gas_time;
    }
}

void State::startAction(const BuildAction& act)
{
    // Checking dependencies
    assert(isSatisfied(act.dependencies, false));

    time_t t = currentTime + act.cost.time;
    if (t > finishTime)
    {
        finishTime = t;
    }

    const BuildCost& cost = act.cost;

    // Consume minerals, gas and supply
    assert(minerals >= cost.minerals*RESS_FACTOR);
    minerals -= cost.minerals*RESS_FACTOR;
    assert(gas >= cost.gas*RESS_FACTOR);
    gas -= cost.gas*RESS_FACTOR;
    supply_used += cost.supply;
    assert(supply_used <= supply_max);

    future_supply_max += act.result.supply;
    assert(future_supply_max >= supply_max);

    // and energy
    assert(energyManager.timeUntilEnergyIsAvailable(cost.energyFrom, cost.energyAmount) == 0);
    energyManager.consumeEnergy(cost.energyFrom, cost.energyAmount);

    // Remove the unit cost
    for (std::pair<action_t, int> unit : cost.units)
    {
        assert(entities[unit.first] >= unit.second);
        entities[unit.first] -= unit.second;
    }

    // Borrow some units
    bool borrowFound = act.borrows.empty();
    action_t borrowedAction = -1;
    for (std::pair<action_t, int> borrow : act.borrows)
    {
        action_t type = borrow.first;
        int count = borrow.second;

        if (entities[type] - borrowed[type] >= count)
        {
            borrowFound = true;
            borrowedAction = type;
            borrowed[type] += count;
            assert(borrowed[type] <= entities[type]);
            break;
        }
    }
    assert(borrowFound);

    // We mark the results as being produced
    for (std::pair<action_t, int> result : act.result.units)
    {
            producing[result.first] += result.second;
    }

    // TODO mabye handle some special stuff here

    ActiveAction aa(t, &act, borrowedAction);
    activeActions.push(aa);
    
    LOG_DEBUG("inserted new action int queue with id: " << act.id << " finish time: " << aa.timeFinished);
}

void State::addActionResult(const BuildResult& res, bool removeProducing)
{
    minerals += res.minerals * RESS_FACTOR;
    gas += res.gas * RESS_FACTOR;

    supply_max += res.supply;
    for (auto unit : res.units)
    {
        addUnit(unit.first, unit.second);
        if (removeProducing)
        {
            producing[unit.first] -= unit.second;
            assert(producing[unit.first] >= 0);
        }
    }
}

int State::getEntityCount(action_t entity)
{
    return entities[entity];
}

time_t State::getTimeTillAllActionsAreFinished() const
{
    return finishTime - currentTime;
}

void State::addUnit(action_t type, int count)
{
    entities[type] += count;
    remembered[type] = 1;

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

    energyManager.advanceTime(t);
}

bool State::isSatisfied(const vector<std::pair<action_t, int>>& constraints, bool use_producing)
{
    for (auto entity : constraints)
    {
        action_t type = entity.first;
        int count = entity.second;

        if (remembered[type] + producing[type] * use_producing < count)
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

ostream& operator<<(ostream& out, State& obj)
{
    out << "State with: " << endl;
    out << "\tMinerals: " << obj.minerals << " (~" << (obj.minerals/RESS_FACTOR) << ")" << endl;
    out << "\tMinerals/tick: " << obj.getMineralsPerTick() << " (~" << (obj.getMineralsPerTick()/RESS_FACTOR) << ")" << endl;
    out << "\tGas: " << obj.gas << " (~" << (obj.gas/RESS_FACTOR) << ")" << endl;
    out << "\tGas/tick: " << obj.getGasPerTick() << " (~" << (obj.getGasPerTick()/RESS_FACTOR) << ")" << endl;
    out << "\tSupply: " << obj.supply_used << "/" << obj.supply_max << endl;
    out << "\tFuture supply max: " << obj.future_supply_max << endl;
    out << "\tWorkers: " << obj.workersAll << "/" << obj.workersMinerals << "/" << obj.workersGas << endl;
    out << "\tCurrent time: " << obj.currentTime << endl;

    for (auto e : obj.entities)
    {
        if (e.second)
        out << "\tEntity: " << e.second << ":" << ConfigParser::Instance().getAction(e.first).name << "(id: " << e.first << ")" << endl;
    }
    for (auto b : obj.borrowed)
    {
        if (b.second)
        out << "\tBorrowed: " << ConfigParser::Instance().getAction(b.first).name << ":" << b.second << endl;
    }
    for (auto p : obj.producing)
    {
        if (p.second)
        out << "\tProducing: " << ConfigParser::Instance().getAction(p.first).name << ":" << p.second << endl;
    }

    out << "\tThere are currently " << obj.activeActions.size() << " active actions:" << endl;

    auto copy = obj.activeActions;

    while (not copy.empty())
    {
        State::ActiveAction aa = copy.top();
        copy.pop();

        out << "\t\t" << aa.action->name << " (id: " << aa.action->id << "/at: " << aa.action << ") " << " finished at: " << aa.timeFinished << endl;
    }

    return out;
}
time_t State::getTimeTillNextActionIsFinished() const
{
    if (activeActions.empty())
    {
        return 0;
    }
    else
    {
        time_t t = activeActions.top().timeFinished - currentTime;
        assert(t > 0);
        return t;
    }
}

void EnergyManager::registerNew(action_t type, energy_t startingEnergy, energy_t max)
{
        maxEnergy[type] = max;
        savedEnergy[type].push_back(startingEnergy);
}

void EnergyManager::consumeEnergy(action_t type, energy_t amount)
{
        vector<energy_t> &vec = savedEnergy.at(type);

        auto it = find_if(vec.begin(), vec.end(),
                        [amount](energy_t e) { return e >= amount;} 
                );

        assert(it != vec.end());

        *it -= amount;
}

time_t EnergyManager::timeUntilEnergyIsAvailable(action_t type, energy_t amount) const
{
        const vector<energy_t> &vec = savedEnergy.at(type);
        assert(!vec.empty());

        auto max_it = max_element(vec.begin(), vec.end());

        energy_t energyNeeded = amount - *max_it;
        if (energyNeeded < 0) energyNeeded = 0;

        return ceil(energyNeeded / ENERGY_PER_TICK);
}

void EnergyManager::advanceTime(time_t amount)
{
        for (auto p : savedEnergy)
        {
                action_t type = p.first;

                // We access again to obtain a reference
                for (energy_t &e : savedEnergy.at(type))
                {
                        e += amount * ENERGY_PER_TICK;
                        if (e > maxEnergy[type])
                        {
                                e = maxEnergy[type];
                        }
                }
        }

        currentTime += amount;
}

ostream& operator<<(ostream& out, EnergyManager& obj)
{
        out << "EnergyManager with: " << endl;
        out << "Currently at time: " << obj.currentTime << endl;
        for (auto p : obj.savedEnergy)
        {
                action_t type = p.first;
                const BuildAction &act = ConfigParser::Instance().getAction(type);
                vector<energy_t> &vec = p.second;

                out << "\tAction: " << act.name << endl;
                out << "\tMaxEnergy: " << obj.maxEnergy[type] << endl;
                out << "\tSavedEnergy: " << endl;

                for (energy_t e : vec)
                {
                        out << "\t\t" << e << endl;
                }
        }
        return out;
}
