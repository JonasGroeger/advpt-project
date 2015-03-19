#include "GeneticOptimizer.h"

GeneticOptimizer::GeneticOptimizer(OptimizationStrategy strategy, action_t targetUnit)
{
    target = targetUnit;
    if (getConfigBoolean(GENETIC_SECTION, FIELD_TIME_SEED, false))
    {
        time_t seed = time(NULL);
        LOG_DEBUG("Using random seed: " << seed);
        srand(seed);
    }
    else
    {
        time_t seed = getConfigInteger(GENETIC_SECTION, FIELD_RANDOM_SEED, 0);
        cerr << "Using static seed: " << seed << endl;
        srand(seed);
    }

    LOG_DEBUG("Initialized Genetic Optimizer with Strategy "<<strategy<<" for target ["
            << ConfigParser::Instance().getAction(target).name);
    generateRandomBuildLists(getConfigInteger(GENETIC_SECTION, FIELD_INITIAL_START_LISTS, 20));
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
        unsigned int randomEntries = getConfigInteger(GENETIC_SECTION, FIELD_INITIAL_RANDOM_ENTRIES, 10);

        for(unsigned int step = 0; step < randomEntries; step++)
        {
            //get a random action of the current race
            int randomAction = rand() % actionCount + firstActionId;
            //get a random position in the range of the actual buildorder size
            int position = rand() % tmp.getSize();
            tmp.insertActionIfPossible(randomAction, position);
        }
        LOG_DEBUG("Created random BuildList ["<<i+1<<"/"<<numberOfLists<<"] with [" << tmp.getSize() << "] entries");
    }
}



