#include "GeneticOptimizer.h"

GeneticOptimizer::GeneticOptimizer(OptimizationStrategy strategy, action_t targetUnit)
{
    target = targetUnit;
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
        unsigned int randomEntries = rand() % 20; //TODO config here

        for(unsigned int step = 0; step < randomEntries; step++)
        {
            int rndRange = rand() % ConfigParser::Instance().getActionCount() + ConfigParser::Instance().getFirstActionId();
            int position = rand() % tmp.getSize();

            tmp.insertActionIfPossible(rndRange, position);
        }

        std::cout << tmp << std::endl;
    }
}



