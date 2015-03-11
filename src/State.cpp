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
    // TODO those are tricky if we want to handle duplicates in act.dependencies/burrows etc. correctly
    // Dependencies
    for (action_t dep : act.dependencies)
    {
        
    }
    // Borrows
    // Costs
    const BuildCost& cost = act.cost;
    if (cost.minerals > 0 && !producingMinerals) return false;
    if (cost.gas > 0 && !producingGas) return false;
    if (cost.supply > (supply_max - supply_used) && !producingSupply) return false;
}

void State::advanceTime(time_t amount) const
{
    time_t end_time = currenTime + amount;

    // Finish all actions that will end withing @amount
    // TODO read priority_queue docs
    // TODO is <= correct?
    while (activeActions.front().timeFinished <= currentTime + amount)
    {
        time_t time_delta = activeActions.front().timeFinished - currentTime;
        assert(time_delta >= 0);
        // First add Ressources
        increaseRessources(time_delta);
        // Increase time
        currentTimme += time_delta;

        // Handle action results
        const Buildact& act = activeActins.front().action.act;
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

        const BuildResult& res = act.result;

        minerals += res.minerals;
        gas += res.gas;
        supply_max += res.supply;

        for (action_t unit : res.units)
        {
            entities[unit]++;
            producing[unit]--;
        }
    }

    increaseRessources(currentTime - time_end);
    currentTime = time_end;
}

// TODO this one is tricky because we need to identify the actions upon which we have to wait
// Also for determining how long until ressources are available we have to take active actions into account that are producing workers which will increase ressource production
time_t State::whenIsPossible(const BuildCost& cost) const
{
    return 0;
}

void State::startAction(const BuildAction& act)
{


    ActiveAction aa(currentTime + act.cost.time, act);
    activeActions.add(aa); // TODO find right function
}

void State::increaseRessources(time_t t)
{
    std::assert(t >= 0);

    minerals += t * MINERALS_PER_TIME_UNIT * workersMinerals;
    gas += t * GAS_PER_TIME_UNIT * workersGas;
}
