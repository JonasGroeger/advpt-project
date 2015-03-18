#include "GeneticOptimizer.h"

GeneticOptimizer::GeneticOptimizer(OptimizationStrategy strategy, action_t targetUnit)
{
    target = targetUnit;
    if (getConfigBoolean("Genetic", "UseTimeSeed", false))
    {
        time_t seed = time(NULL);
        cerr << "Using random seed: " << seed << endl;
        srand(seed);
    }
    else
    {
        time_t seed = getConfigInteger("Genetic", "RandomSeed", 0);
        cerr << "Using static seed: " << seed << endl;
        srand(seed);
    }

    LOG_DEBUG("Initialized Genetic Optimizer with Strategy "<<strategy<<" for target ["
            << ConfigParser::Instance().getAction(target).name);
    generateRandomBuildLists(20);
}

void GeneticOptimizer::generateRandomBuildLists(unsigned int numberOfLists)
{
    BuildOrder minimalBuildList(ConfigParser::Instance().getAction(target).name);
    auto actionsOfRace = ConfigParser::Instance().getAllActions();

    for(unsigned int i = 0; i < numberOfLists; i++)
    {
        //First create a minimal list to make sure we have all dependencies
        BuildOrder tmp(minimalBuildList);
        unsigned int randomEntries = getConfigInteger("Genetic", "InitialRandomEntries", 10);

        for(unsigned int step = 0; step < randomEntries; step++)
        {
            int rndRange = rand() % ConfigParser::Instance().getActionCount() + ConfigParser::Instance().getFirstActionId();
            int position = rand() % tmp.getSize();

            std::cerr << "Trying to insert action ["<<ConfigParser::Instance().getAction(rndRange).name << "] at position ["<<position <<"]" << std::endl;
            tmp.insertActionIfPossible(rndRange, position);
        }

        std::cerr << tmp << std::endl;
    }
}



