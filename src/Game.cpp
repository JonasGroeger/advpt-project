#include "Game.hpp"

bool Game::executeBuildStep(BuildStep* step)
{
    std::cout << "[TIME " << currentState.getSimulationTime()<< "] ";
    std::cout << "[Minerals " << currentState.getMinerals()<< "] ";
    // Switch is not possible here
    if (step->getType() == BuildStepType::UPGRADE)
    {
            std::cout << "upgrading a: ";
    }
    else if (step->getType() == BuildStepType::PRODUCE)
    {
            std::cout << "producing a: ";
            auto producers = currentState.getProducers();
            std::cout << producers.size();
            for (auto it = producers.begin(); it != producers.end(); it++)
            {
                Producer* prod = *it;
                if (prod->canProduce(step->getWhich(), currentState))
                {
                    prod->produce(step->getWhich(), currentState);
                    std::cout << BuildStep::entityTypeToString[step->getWhich()] << std::endl;
                    std::cout << "SUCCESS" << std::endl;
                    return true;
                }
            }
    }
    else if (step->getType() == BuildStepType::CONSTRUCT)
    {
            std::cout << "constructing a: ";
            // TODO
    }
    else if (step->getType() == BuildStepType::CHRONO_BOOST)
    {
            std::cout << "chrono boosting a: ";
            // TODO
    }
    std::cout << BuildStep::entityTypeToString[step->getWhich()] << std::endl;
    std::cout << "FAILURE" << std::endl;

    return false;
}

void Game::loop()
{
    // TODO add a maximum number of steps
    while (!buildOrder.isDone())
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
                std::cout << "advance" << std::endl;
            }
            else
            {
                // Or let the simulation advance first
                break;
            }
        }

        // We update each updateable
        auto updatables = currentState.getUpdatables();

        std::for_each(updatables.begin(), updatables.end(), 
                [this] (Updatable* updt) { updt->update(this->currentState); }
        );

        currentState.incrementSimulationTime();
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
    :currentState(GameState(50, 50, 50)), buildOrder(BuildOrder(file))
{
    currentState.addEntity(TERRAN_SCV, 5);
    currentState.addEntity(TERRAN_COMMAND_CENTER, 5);
}
