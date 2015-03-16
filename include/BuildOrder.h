#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
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
    /*
    * Initializes an empty BuildOrder
    */
    BuildOrder(){};

    /*
    * Initializes a BuildOrder with a minimal BuildList for the action given by target
    * @param target : the target action we want to build.
    */
    BuildOrder(string target)
    {
        createMinimalBuildOrder(target);
    };

    /*
    * Initializes a given BuildOrder given by the values by @other.
    */
    BuildOrder(const BuildOrder& other)
        :availableUnits(other.availableUnits)
        ,buildList(other.buildList)
    {};

    /*
    *
    */
    vector<action_t> getPossibleNextActions(const map<action_t, int> &currUnits, const vector<action_t> &actions);

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
    map<action_t, int> availableUnits;
    vector<action_t> buildList;

    /*applys the buildlist until buildList[pos-1] and returns the resulting unitMap to the caller
    * @returns the resulting unitMap
    */
    map<action_t, int> applyBuildOrderUntilPos(unsigned int pos);

    /*checks if buildList[pos] to buildList.end() is possible with a given unitMap
    * @returns true if it is possible, false otherwise
    */
    bool checkBuildOrderFromPos(map<action_t, int> &units, int supply, int pos);
    /*
    * this one resets all internally used variables
    */
    void reset();
    /*
    * check if a given action is possible with the given values
    * @param currentUnits   : contains an entry for each available action_t and the count as value
    * @param currentSupply  : the currently available supply
    * @param action         : the action to check if its execution is possible
    * @returns true if action is possible, false otherwise
    */
    bool isActionPossible(const map<action_t, int> &currentUnits, unsigned int currentSupply, action_t action);

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
    */
    void getDependencies(action_t id, vector<action_t> &outVector);

    /*
    * Calculates the remaining supply of this BuildOrder after building units from step[0] to step[pos-1].
    * @param index    : the last position to check
    * @returns the resulting supply value as int
    */
    int getSupply(unsigned int index);

    /*
    * Apply the cost/result supply of @action to a given value.
    * @param currSupply : the value to apply the action on
    * @param action     : the action_t to apply
    * @returns the resulting supply value with applied action_t
    */
    int applySupply(unsigned int currSupply, action_t action);

    /*
    * Checks if @unit is already present in @unitMap, if so it will increment the unit count
    * which is represented as the value inside the map.
    * If @unit is not available inside the map, it is inserted with value 1.
    * @param unitMap    : a pointer to the map
    * @param unit       : the action_t to look for
    */
    void addOrIncrementUnit(map<action_t, int> &unitMap, action_t unit);

    /*
    * Checks if @currentSupply >= costSupply
    * @returns true if currentSupply >= costSupply, false otherwise
    */
    bool checkSupply(unsigned int costSupply, unsigned int currentSupply);

    /*
    * Checks if the dependencies given by the @dependencies vector are fulfilled by the map @currentUnits.
    * That means, that for each @dependency, the corresponding entry must be available as a key in @currentUnits.
    * @returns true if dependencies are met, false otherwise
    */
    bool checkDependencies(const vector<std::pair<action_t, int>> &dependencies, const map<action_t, int> &currentUnits);

    /*
    * Checks if the dependencies given by the @borrows vector are fulfilled by the map @currentUnits.
    * That means, that for each @dependency, the corresponding entry must be available as a key in @currentUnits.
    * @returns true if dependencies are met, false otherwise
    */
    bool checkBorrows(const vector<std::pair<action_t, int>> &borrows, const map<action_t, int> &currentUnits);

    /*
    * Checks, that there are no more units of a type than allowed in @currentUnits.
    * @returns true, if no more than allowed units are built
    */
    bool checkMaxUnits(unsigned int maximumUnitsOfAType, action_t action, const map<action_t, int> &currentUnits);
};
