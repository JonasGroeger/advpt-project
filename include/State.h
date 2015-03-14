#pragma once

#include <map>
#include <queue> 
#include <cassert> 

// TODO remove
#include <iostream>
using namespace std;

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
    friend ostream& operator<<(ostream& out, State& obj);
    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals = 0, gas = 0;
    ress_t supply_used = 0, supply_max = 0;

    private:
    // At every position i, entities[i] indicates how many entities with action id i exist currently
    std::map<action_t, int> entities;
    // At every position i, borrowed[i] indicates how many entities with action id i are currently borrowed
    std::map<action_t, int> borrowed;
    // At every position i, producing[i] indicates how many entities with action id i are currently being produced
    // Note: activeActions contains more information on currently produced actions
    std::map<action_t, int> producing;

    /*
     * This simply represents an action that is currently being produced
     * TODO I just overloaded functions until the compiler stopped complaining
     */
    class ActiveAction
    {
        public:
        //this value is the currTime the action was startet + the time this action needs to finish
        time_t timeFinished;
        const BuildAction* action;

        ActiveAction(time_t t, const BuildAction* ba) : timeFinished(t), action(ba) {}
    };

    class ReverseActiveActionComparator
    {
        public:
        bool operator()(const ActiveAction& lhs, const ActiveAction& rhs)
        {
            return lhs.timeFinished > rhs.timeFinished;
        }
    };

    std::priority_queue<ActiveAction, vector<ActiveAction>, ReverseActiveActionComparator> activeActions;


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
    // Paper S.3 "Action Legality"
    /*
     * Returns true iff the following hold true:
     *  - Dependencies and borrows are available, borrowed or being created
     *  - Costs are available or will be created
     */
    bool isLegalAction(const BuildAction&);

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
    time_t isAdditionalTimeNeeded(const BuildAction& cost);

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
     * Besides incrementing the entities map the function will also adjust worker and gasHarvester counts
     */
    void addUnit(action_t type, int count = 1);
    
    // TODO make private
    public:
    void increaseRessources(time_t);
    /*
     * Returns true if all entries in @entities are satisfied
     * If @use_producing is true, untis that are currently being built are also taken into account.
     */
    bool isSatisfied(const vector<std::pair<action_t, int>>& entities, bool use_producing);
    bool hasEnoughSupply(ress_t supply_needed) const;
    ress_t getMineralsPerTick() const;
    ress_t getGasPerTick() const;

    /*
     * This function redistributes the workers to mine minerals and gas.
     * If possible at least one worker will mine minerals, after that as many as possible will mine gas
     */
    void reallocateWorkers();
};

