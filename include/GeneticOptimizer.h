#pragma once

#include "Strategy.h"
#include "Debug.h"
#include "BuildOrder.h"

class GeneticOptimizer
{
public:
    GeneticOptimizer(OptimizationStrategy strategy, action_t target);
    void run();
private:
    action_t target;
    vector<BuildOrder&> buildlists;

    generateRandomBuildLists(unsigned int count);
};
