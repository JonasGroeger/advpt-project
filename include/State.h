#pragma once

#include <algorithm>
#include <map>
#include <queue> 
#include <vector> 
#include <cassert> 
#include <cmath> 

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
using ress_t = long int;
const int RESS_FACTOR = 100; // Scaling factor
using energy_t = long double;

/*
 * These values represent how much minerals/gas are produced by one worker
 */
const ress_t MINERALS_PER_TIME_UNIT = 0.7 * RESS_FACTOR;
const ress_t GAS_PER_TIME_UNIT = 0.35 * RESS_FACTOR;

class EnergyManager
{
    private:
    const long double ENERGY_PER_TICK = 0.5625;
    time_t currentTime = 0;
    map<action_t, vector<energy_t>> savedEnergy;
    map<action_t, energy_t> maxEnergy;

    public:
    void registerNew(action_t type, energy_t startingEnergy, energy_t maxEnergy);
    void consumeEnergy(action_t type, energy_t amount);
    time_t timeUntilEnergyIsAvailable(action_t type, energy_t amount) const;
    void advanceTime(time_t amount);

    /*
     * This has to be defined because of the const member
     * See http://en.cppreference.com/w/cpp/language/as_operator#Deleted_implicitly-declared_copy_assignment_operator
     */
    EnergyManager& operator=(const EnergyManager& rhs)
    {
            this->currentTime = rhs.currentTime;
            this->savedEnergy = rhs.savedEnergy;
            this->maxEnergy = rhs.maxEnergy;

            return *this;
    }

    friend ostream& operator<<(ostream& out, const EnergyManager& obj);
    friend void testEnergyManager();
};

class State {
    //I think it is fine to expose these fields without getter and setters
    public: 
    friend ostream& operator<<(ostream& out, State& obj);
    friend int main(int argc, char*argv[]);
    friend void testTerran();

    // The elapsed time
    time_t currentTime = 0;

    // The available ressources
    ress_t minerals = 0, gas = 0;
    ress_t supply_used = 0, supply_max = 0;
    ress_t future_supply_max = 0;

    private:
    // At every position i, entities[i] indicates how many entities with action id i exist currently
    std::map<action_t, int> entities;
    // At every position i, borrowed[i] indicates how many entities with action id i are currently borrowed
    std::map<action_t, int> borrowed;
    // At every position i, producing[i] indicates how many entities with action id i are currently being produced
    // Note: activeActions contains more information on currently produced actions
    std::map<action_t, int> producing;
    // This is used to remember entities for dependency resolution
    std::map<action_t, int> remembered;

    EnergyManager energyManager;
    /*
     * This simply represents an action that is currently being produced
     */
    class ActiveAction
    {
        public:
        //this value is the currTime the action was startet + the time this action needs to finish
        time_t timeFinished;
        const BuildAction* action;
        action_t borrowedAction = -1;

        ActiveAction(time_t t, const BuildAction* a, action_t ba) : timeFinished(t), action(a), borrowedAction(ba) {}
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
    int workersAll = 0;
    // How many are producing minerals
    int workersMinerals = 0;
    // How many mules are harvesting minerals
    int activeMules = 0;
    // How many are producing gas
    int workersGas = 0;

    // How many gas slots are available
    // This is increase by 3 for every gas harvester
    int gasHarvesting = 0;

    time_t finishTime = 0;

    public:
    State(){};
    State(const map<action_t, int> &startConfig);

    bool operator==(const State &rhs) const;
    bool operator!=(const State &rhs) const;

    // Paper S.3 "Action Legality"
    /*
     * Returns true iff the following hold true:
     *  - Dependencies and borrows are available, borrowed or being created
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

    void addActionResult(const BuildResult&, bool removeProducing=true);

    int getEntityCount(action_t entity);
    time_t getTimeTillAllActionsAreFinished() const;

    private:
    /*
     * Adds @count units of typed @type
     * Besides incrementing the entities map the function will also adjust worker and gasHarvester counts
     * NOTE: you probably want to use addActionResults
     */
    void addUnit(action_t type, int count = 1);
    
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
     * I think this qualifies for the longest function name of this project!
     */
    time_t getTimeTillNextActionIsFinished() const;
    /*
     * This function redistributes the workers to mine minerals and gas.
     * If possible at least one worker will mine minerals, after that as many as possible will mine gas
     */
    void reallocateWorkers();
};

