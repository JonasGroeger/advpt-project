#pragma once

#include "BuildAction.h"

class State {
    
    public: 
    // The elapsed time
    int currentTime = 0;

    // The available ressources
    int minerals, gas;
    int supply_used, supply_max;

    private:
    vector<int> entities;
    vector<bool> borrowed;

    // TODO 
    // PriorityQueue aus aktiven Actions

    // TODO Workers are defined implicitly

    public:
    /*
     * Tries to borrow an entity that is produced by act
     * Returns false if there is no such entity that is not already borrowed 
     * Otherwise true
     */
    bool borrow(actiont act);
    bool unborrow(actiont act);

    // Paper S.3 "Action Legality"
    /*
     * Returns true iff the following hold true:
     *  - Dependencies and borrows are available, borrowed or being created
     *  - Costs are available or will be created
     */
    bool isLegalAction(const BuildACtion&) const;
    void startAction(const BuildAction&);

    // Paper S.3 "Fast Forward and State Transistion"
    /*
     * S' <- Sim(S, d)
     * This advances the state by @amount without any new actions being issude
     *
     */
    void advanceTime(int amount) const;
};
