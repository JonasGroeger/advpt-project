#include "Game.hpp"

bool Game::executeBuildStep(BuildStep* step)
{
    //std::cerr << "[TIME " << currentState.getSimulationTime()<< "] ";
    //std::cerr << "[Minerals " << currentState.getMinerals()<< "] ";
    // Switch is not possible here
    if (step->getBuildStepType() == BuildStepType::UPGRADE)
    {
            std::cerr << "upgrading a: ";
    }
    else if (step->getBuildStepType() == BuildStepType::PRODUCE)
    {
            auto producers = currentState.getProducers();
            for (auto it = producers.begin(); it != producers.end(); it++)
            {
                Producer* prod = *it;
                if (prod->canProduce(step->getEntityType(), currentState))
                {
                    std::cerr << "[TIME " << currentState.getSimulationTime()<< "] ";
                    std::cerr << "[Minerals " << currentState.getMinerals()<< "] ";
                    std::cerr << "producing a: ";
                    std::cerr << BuildStep::entityTypeToString[step->getEntityType()] << std::endl;
                    prod->produce(step->getEntityType(), currentState);
                    return true;
                }
            }
    }
    else if (step->getBuildStepType() == BuildStepType::CONSTRUCT)
    {
            std::cerr << "constructing a: ";
            // TODO
    }
    else if (step->getBuildStepType() == BuildStepType::CHRONO_BOOST)
    {
            std::cerr << "chrono boosting a: ";
            // TODO
    }
    //std::cerr << BuildStep::entityTypeToString[step->getWhich()] << std::endl;
    //std::cerr << "FAILURE" << std::endl;

    return false;
}

int Game::loop()
{
    while (!buildOrder.isDone() && !currentState.maxSimTimeReached())
    {
        // As long as there are still steps left
        while (!buildOrder.isDone())
        {
            BuildStep* nextStep = buildOrder.getNextStep();

            // We try to execute the first one
            if (executeBuildStep(nextStep))
            {
                // If successfull we try the next step
                buildOrder.advance();
                std::cerr << "advance" << std::endl;
            }
            else
            {
                // Or let the simulation advance first
                break;
            }
        }

        std::cerr << "Update..." << std::endl;

        // We update each updateable
        auto updatables = currentState.getUpdatables();

        std::for_each(updatables.begin(), updatables.end(), 
                [this] (Updatable* updt) { updt->update(this->currentState); }
        );

        currentState.incrementSimulationTime();
    }

    if(currentState.maxSimTimeReached() && ! buildOrder.isDone()){
        //buildlist did not succeed so return non zero
        return -1;
    }

    std::cerr << "All orders are given!" << std::endl;

    /*
     * No every Order is given, but some may still take some time to complete.
     * So we ask every producer how long he will need and wait the maximum amount
     */
    long maxTime = 0;
    auto producers = currentState.getProducers();

    std::for_each(producers.begin(), producers.end(),
            [&maxTime] (Producer* prod) 
            { 
                maxTime = (prod->getTimeToFinish() > maxTime) ? prod->getTimeToFinish() : maxTime;
            }
    );

    std::cerr << "Need to wait " << maxTime << " for everything to finish up." << std::endl;

    for (long i = 0; i < maxTime; i++)
    {
        currentState.incrementSimulationTime();
    }

    std::cerr << "Finished." << std::endl;
    //all fine, return 0
    return 0;
}

bool Game::isFinished()
{
    return (buildOrder.getNextStep() == nullptr);
}

GameState& Game::getFinalState()
{
    return currentState;
}

// TODO remove these debug values of gamestate
Game::Game(char *file)
    :currentState(GameState(1000, 50, 50, 50)), buildOrder(BuildOrder(file))
{
    currentState.addEntity(TERRAN_SCV, 5);
    currentState.addEntity(TERRAN_COMMAND_CENTER, 1);
}
