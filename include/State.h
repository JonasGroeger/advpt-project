#pragma once

#include "BuildAction.h"

using timet = int;
using resst = int;

class State {
    // The elapsed time
    timet currentTime = 0;

    // The available ressources
    resst minerals, gas;
    resst supply_used, supply_max;

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

    bool canStartAction(const BuildAction&) const;
    void startAction(const BuildAction&);
};
