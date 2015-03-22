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
            return lx.execute().executionTime < rx.execute().executionTime;
        }
    };

    struct RushComparator
    {
        int maxTime = getConfigInteger(GENETIC_SECTION, FIELD_RUSH_MAX_TIME, 1000);
        action_t target = getConfigInteger(GENETIC_SECTION, FIELD_TARGET, -1);

        bool operator()( BuildOrder& lx, BuildOrder& rx ) const {
            const ExecutionResult& rl = lx.execute();
            const ExecutionResult& rr = rx.execute();

            if (rl.executionTime > maxTime) return false;
            if (rr.executionTime > maxTime) return true;

            return lx.getUnitCount(target) > rx.getUnitCount(target);
        }
    };

    void generateRandomStartLists(unsigned int numberOfLists);
    void makeChildren(const vector<action_t> &mum, const vector<action_t> &dad, BuildOrder &child);
    void mutate(BuildOrder &child, double probability);
};
