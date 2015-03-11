#pragma once

#include <vector>
#include <queue> 
#include <assert> 

#include "BuildAction.h"
#include "ConfigParser.h"

// TODO better datatype
using ress_t = long double;

// TODO managin units with energy

class State {
    //I think it is fine to expose these fields without getter and setters
    public: 
    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals = 0, gas = 0;
    ress_t supply_used = 0, supply_max = 0;

    private:
    // At every position i, entities[i] indicates how many entities with action id i exist currently
    std::vector<int> entities;
    // At every position i, borrowed[i] indicates how many entities with action id i are currently borrowed
    std::vector<int> borrowed;
    // At every position i, producing[i] indicates how many entities with action id i are currently being produced
    // Note: activeActions contains more information on currently produced actions
    std::vector<int> producing;

    /*
     * This simply represents an action that is currently being produced
     */
    class ActiveAction
    {
        private:
        time_t timeFinished;
        const BuildAction& action;

        public:
        ActiveAction(time_t _timeFinished, const BuildAction& _action)
            : timeFinished(_timeFinished), action(_action) {}

        private:
        /*
         * Reverse ordering on timeFinished so priority_queue works
         */
        bool operator<(const ActiveAction& lhs) const
        {
            return timeFinished > lhs.timeFinished;
        }
    } ;

    std::priority_queue<ActiveAction> activeActions;

    // TODO
    const ress_t MINERALS_PER_TIME_UNIT = 0.1;
    const ress_t GAS_PER_TIME_UNIT = 0.1;

    // How many workers exist at all
    int workersAll;
    // How many are producing minerals
    int workersMinerals;
    // How many are producing gas
    int workersGas;

    // How many gas slots are available
    // This is increase by 3 for every gas harvester
    int gasHarvesting;

    public:
    State(const ConfigParser&);

    // Paper S.3 "Action Legality"
    /*
     * Returns true iff the following hold true:
     *  - Dependencies and borrows are available, borrowed or being created
     *  - Costs are available or will be created
     */
    bool isLegalAction(const BuildAction&) const;

    // Paper S.3 "Fast Forward and State Transistion"
    /*
     * S' <- Sim(S, d)
     * This advances the state by @amount without any new actions being issude
     */
    void advanceTime(time_t amount) const;

    /*
     * d <- When(S, R)
     * Returns the time it will take until the ressource requirement R is met
     */
    time_t whenIsPossible(const BuildCost&) const;

    /*
     * S' <- Do(S, a)
     * Starts an Action assuming all required ressources are available.
     *  - Subtract consumed ressources
     *  - Update actions in progress // TODO what does this mean?
     *  - Flaggin borrowed resources
     *  - Insert the action
     */
    void startAction(const BuildAction&);
    
    private:
    void increaseRessources(time_t);
};
