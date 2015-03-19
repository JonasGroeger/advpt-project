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
        LOG_DEBUG("Using static seed: " << seed);
        srand(seed);
    }

    LOG_DEBUG("Initialized Genetic Optimizer with Strategy "<<strategy<<" for target ["
            << ConfigParser::Instance().getAction(target).name) << "].";

    generateRandomStartLists(getConfigInteger(GENETIC_SECTION, FIELD_INITIAL_START_LISTS, 20));

}

void GeneticOptimizer::generateRandomStartLists(unsigned int numberOfLists)
{
    //First create a minimal list to make sure we have all dependencies
    BuildOrder minimalBuildList(ConfigParser::Instance().getAction(target).name);
    //Get the range of action_t which are used by our race
    unsigned int firstActionId = ConfigParser::Instance().getFirstActionId();
    unsigned int actionCount = ConfigParser::Instance().getActionCount();

    for(unsigned int i = 0; i < numberOfLists; i++)
    {
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
        LOG_DEBUG("Created random BuildList ["<<i+1<<"/"<<numberOfLists<<"] with [" << tmp.getSize() << "] entries.");
        buildlists.push_back(tmp);
    }
}

void GeneticOptimizer::run()
{
    
}



