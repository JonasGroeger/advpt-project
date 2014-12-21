#include "Game.hpp"

bool Game::executeBuildStep(BuildStep* step)
{
    //std::cerr << "[TIME " << currentState.getSimulationTime()<< "] ";
    //std::cerr << "[Minerals " << currentState.getMinerals()<< "] ";
    // Switch is not possible here
    if (step->getBuildStepType() == BuildStepType::UPGRADE)
    {
        auto upgradables = currentState.getUpgradeables();
        for (Upgradable *upgrd : upgradables)
        {
            if (upgrd->upgradeIfPossible(step->getEntityType(), currentState))
            {
                return true;
            }
        }
    }
    else if (step->getBuildStepType() == BuildStepType::PRODUCE)
    {
        auto producers = currentState.getProducers();
        for (auto it = producers.begin(); it != producers.end(); it++)
        {
            Producer* prod = *it;

            //TODO fix produce interface

            if (prod->produceEntityIfPossible(step->getEntityType(), currentState))
            {
                printBuildStartMessage(step->getEntityType());
                return true;
            }
        }
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
    while ( (!buildOrder.isDone() || isAnybodyProducing())
          && !currentState.maxSimTimeReached())
    {
        bool somethingHappened = false;

        /*
         * We update before executing any build steps so any Producer
         * that finishes this tick is cleared to produce something new
         */
        auto updatables = currentState.getUpdatables();
        std::for_each(updatables.begin(), updatables.end(), 
            [this] (Updatable* updt) { updt->update(this->currentState); }
        );

        // As long as there are still steps left
        while (!buildOrder.isDone())
        {
            BuildStep* nextStep = buildOrder.getNextStep();

            // We try to execute the first one
            if (executeBuildStep(nextStep))
            {
                // If successfull we try the next step
                buildOrder.advance();
                somethingHappened = true;
            }
            else
            {
                // Or let the simulation advance first
                break;
            }
        }

        currentState.reassignWorkers();

        if(somethingHappened){
            printResourcesMessage();
            printWorkerMessage();
        }

        currentState.incrementSimulationTime();
    }

    if(currentState.maxSimTimeReached() && !buildOrder.isDone()){
        //buildlist did not succeed so return non zero and print error message
        std::cerr << "Reached maximum Time - aborting..." << std::endl;
        printWorkerMessage();
        printResourcesMessage();
        return -1;
    }
    printResourcesMessage();

    //all fine, return 0
    return 0;
}

bool Game::isAnybodyProducing() const
{
    auto producers = currentState.getProducers();
    bool result = false;
    std::for_each(producers.begin(), producers.end(),
        [&result] (Producer* prod) 
        { 
            if (prod->getTimeToFinish() > 0) result = true;
        }
    );
    return result;
}

bool Game::isFinished()
{
    return (buildOrder.getNextStep() == nullptr);
}

GameState& Game::getFinalState()
{
    return currentState;
}

void Game::printMessageProlog() const
{
    std::cout << std::left;
    std::cout << std::setw(5);
    std::cout << currentState.getSimulationTime();
    std::cout << std::setw(14);
}

void Game::printBuildStartMessage(EntityType type) const
{
    printMessageProlog();
    std::cout << "build-start";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
}

void Game::printBuildEndMessage(EntityType type) const
{
    printMessageProlog();
    std::cout << "build-end";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
    printResourcesMessage(); // TODO this is only for debugging
}

void Game::printWorkerMessage() const
{
    int idleWorkers = 0;
    int vespineWorkers = 0;
    int mineralWorkers = 0;
    int producingWorkers = 0;
    auto workers = currentState.getWorkers();

    for(auto* worker : workers){
        switch(worker->getTypeOfWork()){
            case TypeOfWork::Idle:
                idleWorkers++;
                break;
            case TypeOfWork::Vespine:
                vespineWorkers++;
                break;
            case TypeOfWork::Minerals:
                mineralWorkers++;
                break;
            case TypeOfWork::Producing:
                producingWorkers++;
                break;
        }
    }

    printMessageProlog();
    std::cout << "workers";
    std::cout << "minerals:" << mineralWorkers;
    std::cout << ",vespene:" << vespineWorkers;
    std::cout << std::endl;
}

void Game::printResourcesMessage() const
{
    printMessageProlog();
    std::cout << "resources";
    std::cout << "minerals:" << currentState.getMinerals();
    std::cout << ",vespene:" << currentState.getVespine();
    std::cout << ",usedSupply:" << currentState.getUsedSupply();
    std::cout << ",availableSupply:" << currentState.getAvailableSupply();
    std::cout << std::endl;
}

// TODO remove these debug values of gamestate
Game::Game(char *file)
    :buildOrder(BuildOrder(file))
{
    //TODO maxSimTime == 1000 should fit this assignments requirements
    //
    switch (buildOrder.getRace())
    {
        case PROTOSS:
            currentState.addEntity(PROTOSS_PROBE, 5);
            currentState.addEntity(PROTOSS_NEXUS, 1);
            currentState.addCreatedEntity(WarpHelper::Instance());
            break;
        case TERRAN:
            currentState.addEntity(TERRAN_SCV, 5);
            currentState.addEntity(TERRAN_COMMAND_CENTER, 1);
            break;
        case ZERG:
            currentState.addEntity(ZERG_DRONE, 5);
            currentState.addEntity(ZERG_HATCHERY, 1);
            currentState.addEntity(ZERG_OVERLORD, 1);
            break;
        default:
            throw invalid_argument("Unrecognized race");
            break;
    }

    currentState.setMaxSimTime(3000);
    currentState.addMineralsWithFactor(50 * GameState::FACTOR); 
    currentState.consumeEnoughSupply(5 * 1); // Each Worker consumes 1 supply 

    currentState.registerLogger(this);
}
