#pragma once

#include <vector>
#include <queue> 

#include "BuildAction.h"

using ress_t = int;

class State {
    //I think it is fine to expose these fields without getter and setters
    public: 
    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals, gas;
    ress_t supply_used, supply_max;

    private:
    // We save all produced units/buildings here
    // Duplicates are possible
    std::vector<action_t> entities;
    std::vector<bool> borrowed;

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

    // TODO workers

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
