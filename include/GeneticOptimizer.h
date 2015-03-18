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
private:
    action_t target;
    vector<BuildOrder> buildlists;

    void generateRandomBuildLists(unsigned int numberOfLists);
};
