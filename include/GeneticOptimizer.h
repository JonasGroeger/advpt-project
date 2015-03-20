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

    void generateRandomStartLists(unsigned int numberOfLists);
    void mutate(BuildOrder &child, double probability);
};
