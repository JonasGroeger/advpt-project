#pragma once

#include <vector>
#include <queue> 
#include <cassert> 

#include "BuildAction.h"
#include "ConfigParser.h"
#include "Debug.h"

/*
 * This datatype is used to represent minerals and gas
 * Internally it is always multiplied by RESS_FACTOR 
 */
// TODO if someone is bored this could also be implementated by a smart class with overloaded arithmetic operators and default int conversions etc.
using ress_t = long double;
const int RESS_FACTOR = 100; // Scaling factor

/*
 * These values represent how much minerals/gas are produced by one worker
 */
const ress_t MINERALS_PER_TIME_UNIT = 0.7 * RESS_FACTOR;
const ress_t GAS_PER_TIME_UNIT = 0.35 * RESS_FACTOR;

// TODO managing units with energy

class State {
    //I think it is fine to expose these fields without getter and setters
    public: 
    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals = 0, gas = 0;
    // TODO handle supply with a factor??
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
     * TODO I just overloaded functions until the compiler stopped complaining
     */
    class ActiveAction
    {
        public:
        time_t timeFinished;
        BuildAction action;

        ActiveAction(time_t _timeFinished, BuildAction _action)
            : timeFinished(_timeFinished), action(_action) {}
        ActiveAction(const ActiveAction& lhs)
            : timeFinished(lhs.timeFinished), action(lhs.action) {}

        /*
         * Reverse ordering on timeFinished so priority_queue works
         */
        bool operator<(const ActiveAction& lhs) const
        {
            return timeFinished > lhs.timeFinished;
        }

        ActiveAction& operator=(const ActiveAction& lhs)
        {
            this->timeFinished = lhs.timeFinished;
            this->action = lhs.action;
            return *this;
        }
    };

    std::priority_queue<ActiveAction> activeActions;


    // How many workers exist at all
    int workersAll;
    // How many are producing minerals
    int workersMinerals;
    // How many are producing gas
    int workersGas;

    // How many gas slots are available
    // This is increase by 3 for every gas harvester
    int gasHarvesting;

    // This is used to determine if an Action that consumes gas is legal
    bool willProduceGas = false;

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
    void advanceTime(time_t amount);

    /*
     * d <- When(S, R)
     * IMPORTANT this implementation differs from the paper
     *
     * Returns false if the @cost is satisfied.
     * Otherwise it will return a time amount which should be used to advance the simulation.
     * NOTE it is not guarenteed that @cost will be satisfied after this time amount.
     * You must check again!
     */
    time_t isAdditionalTimeNeeded(const BuildAction& cost) const;

    /*
     * S' <- Do(S, a)
     * Starts an Action assuming all required ressources are available.
     *  - Subtract consumed ressources
     *  - Update actions in progres
     *  - Flaggin borrowed resources
     *  - Insert the action
     */
    void startAction(const BuildAction&);

    /*
     * Adds @count units of typed @type
     */
    void addUnit(action_t type, int count);
    
    // TODO make private
    public:
    void increaseRessources(time_t);
    /*
     * Returns true if all entries in @entities are satisfied
     * If @use_producing is true, untis that are currently being built are also taken into account.
     */
    bool isSatisfied(const vector<std::pair<action_t, int>>& entities, bool use_producing) const;
    bool hasEnoughSupply(ress_t supply_needed) const;
    ress_t getMineralsPerTick() const;
    ress_t getGasPerTick() const;
};
