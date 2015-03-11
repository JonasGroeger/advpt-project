#pragma once

#include "BuildAction.h"

using time_t = int;
using ress_t = int;

class State {
    
    public: 
    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals, gas;
    ress_t supply_used, supply_max;

    private:
    vector<action_t> entities;
    vector<bool> borrowed;

    // TODO 
    // PriorityQueue aus aktiven Actions

    // TODO Workers are defined implicitly

    public:
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
};
