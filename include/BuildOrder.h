#pragma once

#include "BuildAction.h"
#include "ConfigParser.h"
#include <State.h>
#include <iostream>
#include <vector>
#include <algorithm>    // std::reverse
#include <initializer_list>

#include "Debug.h"

using namespace std;

struct ExecutionResult
{
        bool successfull;
        time_t executionTime;
        vector<action_t>::difference_type lastStep;
};

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
    explicit BuildOrder(string target)
    {
        createMinimalBuildOrder(target);
    }

    explicit BuildOrder(std::initializer_list<string> l)
    {
        State state = State(ConfigParser::Instance().getStartConfig());
        buildList.resize(l.size());
        transform(l.begin(), l.end(), buildList.begin(), 
                [] (string s) { return ConfigParser::Instance().getAction(s).id;}
                );
    }

    unsigned int getSize() const;

    action_t getAction(unsigned int position) const;

    void setBuildList(const vector<action_t>& vec);
    const vector<action_t> &getBuildList() const;

    //unsigned int getFitness() const;
    ExecutionResult execute(time_t maxTime = 0) const;

    unsigned int getUnitCount(action_t target) const;
    unsigned int getUnitCount(action_t target, vector<action_t>::difference_type untilStep) const;

    void setTargetUnit(action_t target);
    /*
    *
    */
    vector<action_t> getPossibleNextActions(const vector<action_t> &actions, State& state) const;

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
    vector<action_t> buildList;

    void addOrIncrementUnit(map<action_t, int> &unitMap, action_t unit);
    void startActionInState(action_t actionId, State& state) const;
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
    vector<action_t> getDependencies(action_t id) const;

    /*
    * Checks, that there are no more units of a type than allowed in @currentUnits.
    * @returns true, if no more than allowed units are built
    */
    bool checkMaxUnits(unsigned int maximumUnitsOfAType, action_t action, const map<action_t, int> &currentUnits);
};
