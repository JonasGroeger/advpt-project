#include "GeneticOptimizer.h"

GeneticOptimizer::GeneticOptimizer(OptimizationStrategy strategy, action_t targetUnit)
{
    target = targetUnit;
    if (getConfigBoolean("Genetic", "UseTimeSeed", false))
    {
        time_t seed = time(NULL);
        LOG_DEBUG("Using random seed: " << seed);
        srand(seed);
    }
    else
    {
        time_t seed = getConfigInteger("Genetic", "RandomSeed", 0);
        LOG_DEBUG("Using static seed: " << seed);
        srand(seed);
    }

    LOG_DEBUG("Initialized Genetic Optimizer with Strategy "<<strategy<<" for target ["
            << ConfigParser::Instance().getAction(target).name) << "].";
    generateRandomBuildLists(20);
}

void GeneticOptimizer::generateRandomBuildLists(unsigned int numberOfLists)
{
    BuildOrder minimalBuildList(ConfigParser::Instance().getAction(target).name);
    unsigned int firstActionId = ConfigParser::Instance().getFirstActionId();
    unsigned int actionCount = ConfigParser::Instance().getActionCount();

    for(unsigned int i = 0; i < numberOfLists; i++)
    {
        //First create a minimal list to make sure we have all dependencies
        BuildOrder tmp(minimalBuildList);
        unsigned int randomEntries = getConfigInteger("Genetic", "InitialRandomEntries", 10);

        for(unsigned int step = 0; step < randomEntries; step++)
        {
            //get a random action of the current race
            int randomAction = rand() % actionCount + firstActionId;
            //get a random position in the range of the actual buildorder size
            int position = rand() % tmp.getSize();
            tmp.insertActionIfPossible(randomAction, position);
        }
        LOG_DEBUG("Created random BuildList ["<<i+1<<"/"<<numberOfLists<<"] with [" << tmp.getSize() << "] entries.");
    }
}



