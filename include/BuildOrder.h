#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
#include <State.h>
#include <iostream>
#include <vector>
#include <algorithm>    // std::reverse

#include "Debug.h"

using namespace std;

/*
* @Invariant of this class is that the buildlist is always either empty or a valid buildlist
*/
class BuildOrder
{
public:

    BuildOrder(){};
    /*
    * Initializes a BuildOrder with a minimal BuildList for the action given by target
    * @param target : the target action we want to build.
    */
    BuildOrder(string target)
    {
        state = State(ConfigParser::Instance().getStartConfig());
        createMinimalBuildOrder(target);
    };

    /*
    * Initializes a given BuildOrder given by the values by @other.
    */
    BuildOrder(const BuildOrder& other)
        :availableUnits(other.availableUnits)
        ,buildList(other.buildList)
    {};

    unsigned int getSize();

    unsigned int getFitness();

    unsigned int getUnitCount(action_t action, time_t maxTime);
    /*
    *
    */
    vector<action_t> getPossibleNextActions(const vector<action_t> &actions);

    /*
    * Insert an action at the given position in the buildlist.
    * @param action : the action_t to insert
    * @param position : the zero based index
    * @returns true if action was successful, false otherwise
    */
    bool insertActionIfPossible(action_t action, unsigned int position);

    /*
    * Removes an action at the given index of the buildlist
    * @param position : the zero based index
    * @returns true if action was successful, false otherwise
    */
    bool removeActionIfPossible(unsigned int position);

    /*
    * Replaces an action at the given index of the buildlist
    * @param action : the action_t to insert
    * @param position : the zero based index
    * @returns true if action was successful, false otherwise
    */
    bool replaceActionIfPossible(action_t newAction, unsigned int position);

    friend ostream& operator<< (ostream &out, BuildOrder &obj);

private:
    State state;
    map<action_t, int> availableUnits;
    vector<action_t> buildList;

    void addOrIncrementUnit(map<action_t, int> &unitMap, action_t unit);
    void startActionInState(const action_t &actionId);
    /*applys the buildlist until buildList[pos-1] and returns the resulting unitMap to the caller
    * @returns the resulting unitMap
    */
    bool applyBuildOrder(unsigned int posStart, unsigned int posEnd);

    /*
    * this one resets all internally used variables
    */
    void reset();

    /*
     * Removes all build steps and creates a fresh BuildOrder with all dependencies
     * to produce @target as the last entry.
     * @param target : the target unit to produce
     */
    void createMinimalBuildOrder(string target);

    /*
    * Resolves all dependencies of @id by recursive calls.
    * Writes the dependencies to @outVector.
    * @param id         : the action_t to resolve dependencies for
    * @param outVector reference to the vector which receives the results
    * TODO doku
    */
    vector<action_t> getDependencies(action_t id);

    /*
    * Checks, that there are no more units of a type than allowed in @currentUnits.
    * @returns true, if no more than allowed units are built
    */
    bool checkMaxUnits(unsigned int maximumUnitsOfAType, action_t action, const map<action_t, int> &currentUnits);
};
