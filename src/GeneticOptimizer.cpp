#include "GeneticOptimizer.h"

GeneticOptimizer::GeneticOptimizer(OptimizationStrategy strategy, action_t targetUnit)
{
    target = targetUnit;
    mode = strategy;
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
            << ConfigParser::Instance().getAction(target).name << "].");

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
            if(randomAction == target)
            {
                continue;
            }
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
    switch(mode)
    {
        case OptimizationStrategy::Push:
            runPush();
            break;
        case OptimizationStrategy::Rush:
            runRush();
            break;
    }
}

void GeneticOptimizer::runRush()
{
    int generations = getConfigInteger(GENETIC_SECTION, FIELD_NUMBER_OF_GENERATIONS, 10);

    double fraction = ((double) 1) / buildlists.size();
    double probabilityToMutate = 0;

    std::cout << "Starting optimizer ...." << std::endl;
    for(int generation = 0; generation < generations; generation++)
    {
        if(probabilityToMutate >= 1) probabilityToMutate = 0;

        std::sort(buildlists.begin(), buildlists.end(), RushComparator());
        //keep the two best lists
        const auto& mum = buildlists[0].getBuildList();
        const auto& dad = buildlists[1].getBuildList();

        probabilityToMutate += fraction;

        for(auto iter = buildlists.begin()+2; iter < buildlists.end(); iter++)
        {
            BuildOrder &child = *iter;

            makeChildren(mum, dad, child);

            mutate(child, probabilityToMutate);
        }
        std::cout << "\r Current Generation ["<<(generation+1)<<" / " << generations <<"] best Fitness ["<< buildlists[0].getUnitCount(target) <<"]";
        std::cout.flush();
    }
    std::sort(buildlists.begin(), buildlists.end(), PushComparator());

    std::cout << "\nWinner with fitness of [" << buildlists[0].getUnitCount(target) << "] took time ["<<buildlists[0].execute().executionTime<<"]" << std::endl;
    std::cout << buildlists[0] << std::endl;
    std::cout << " Probablity = "<< std::to_string(probabilityToMutate) << std::endl;
}

void GeneticOptimizer::runPush()
{
    int generations = getConfigInteger(GENETIC_SECTION, FIELD_NUMBER_OF_GENERATIONS, 10);

    double fraction = ((double) 1) / buildlists.size();
    double probabilityToMutate = 0;

    std::cout << "Starting optimizer ...." << std::endl;
    for(int generation = 0; generation < generations; generation++)
    {
        if(probabilityToMutate >= 1) probabilityToMutate = 0;

        std::sort(buildlists.begin(), buildlists.end(), PushComparator());

        //keep the two best lists
        auto mum = buildlists[0].getBuildList();
        auto dad = buildlists[1].getBuildList();

        probabilityToMutate += fraction;

        for(auto iter = buildlists.begin()+2; iter < buildlists.end(); iter++)
        {
            BuildOrder &child = *iter;

            makeChildren(mum, dad, child);

            mutate(child, probabilityToMutate);
        }
        std::cout << "\r Current Generation ["<<(generation+1)<<" / " << generations <<"] best Fitness ["<< buildlists[0].execute().executionTime <<"]";
        std::cout.flush();
    }
    std::sort(buildlists.begin(), buildlists.end(), PushComparator());
    std::cout << "\nWinner with fitness of [" << buildlists[0].execute().executionTime << "]" << std::endl;
    std::cout << buildlists[0] << std::endl;
    std::cout << " Probablity = "<< std::to_string(probabilityToMutate) << std::endl;
}

void GeneticOptimizer::makeChildren(const vector<action_t> &mum, const vector<action_t> &dad, BuildOrder &child)
{
    bool targetAdded = false;
    //Do the recombination of the parent "genoms" until we could add our target
    while(!targetAdded)
    {
        //clear the child
        child = BuildOrder();

        auto mumIter = mum.begin();
        auto dadIter = dad.begin();
        int pos = 0;

        while (mumIter != mum.end() && dadIter != dad.end())
        {
            action_t action = (rand() % 2 == 0)
                    ? *mumIter
                    : *dadIter;

            if (child.insertActionIfPossible(action, pos))
            {
                if (action == target) targetAdded = true;
                pos++;
            }
            mumIter++;
            dadIter++;
        }

        if (!targetAdded)
        {
            while (mumIter != mum.end())
            {
                if (child.insertActionIfPossible(*mumIter, pos))
                {
                    if (*dadIter == target) targetAdded = true;
                    pos++;
                }
                mumIter++;
            }
            while (dadIter != dad.end())
            {
                if (child.insertActionIfPossible(*dadIter, pos))
                {
                    if (*dadIter == target) targetAdded = true;
                    pos++;
                }
                dadIter++;
            }
        }
    }
}

void GeneticOptimizer::mutate(BuildOrder &child, double probability)
{
    //we dont want to mutate our target (last entry)
    for(unsigned int i = 0; i < child.getSize()-1; i++)
    {
        double mutationProbability = (double)rand() / RAND_MAX;

        if(mutationProbability <= probability)
        {
            //Get the range of action_t which are used by our race
            unsigned int firstActionId = ConfigParser::Instance().getFirstActionId();
            unsigned int actionCount = ConfigParser::Instance().getActionCount();
            int randomAction = rand() % actionCount + firstActionId;

            //We dont want to add our target action more than once
            if(randomAction == target && mode == OptimizationStrategy::Push)
            {
                continue;
            }

            // TODO sadf
            int mutationAction = rand()%3;
            switch(mutationAction)
            {
                case 0:
                    child.removeActionIfPossible(i);
                    break;
                case 1:
                    child.replaceActionIfPossible(randomAction, i);
                    break;
                case 2:
                    child.insertActionIfPossible(randomAction, i);
                    break;
            }
        }
    }
}




