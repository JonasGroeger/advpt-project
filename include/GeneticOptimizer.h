#pragma once

#include <ctime>

#include "Strategy.h"
#include "Debug.h"
#include "BuildOrder.h"
#include "Config.h"

class GeneticOptimizer
{
public:
    GeneticOptimizer(OptimizationStrategy strategy, action_t target);
    void run();
    void runPush();
    void runRush();
private:
    OptimizationStrategy mode;
    action_t target;
    vector<BuildOrder> buildlists;

    struct PushComparator
    {
        bool operator()( BuildOrder& lx, BuildOrder& rx ) const {
            return lx.getFitness() < rx.getFitness();
        }
    };

    struct RushComparator
    {
        int maxTime = getConfigInteger(GENETIC_SECTION, FIELD_RUSH_MAX_TIME, 1000);
        bool operator()( BuildOrder& lx, BuildOrder& rx ) const {
            return lx.getUnitCount(maxTime) > rx.getUnitCount(maxTime);
        }
    };

    void generateRandomStartLists(unsigned int numberOfLists);
    void makeChildren(const vector<action_t> &mum, const vector<action_t> &dad, BuildOrder &child);
    void mutate(BuildOrder &child, double probability);
};
