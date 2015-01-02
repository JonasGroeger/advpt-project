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
                currentState.printBuildStartMessage(step->getEntityType());
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
                currentState.printBuildStartMessage(step->getEntityType());
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
#ifdef DEBUG
            currentState.printResourcesMessage();
#endif
            currentState.printWorkerMessage();
        }

        currentState.incrementSimulationTime();
    }

    if(currentState.maxSimTimeReached() && !buildOrder.isDone()){
        //buildlist did not succeed so return non zero and print error message
        std::cerr << "Reached maximum Time - aborting..." << std::endl;
        currentState.printWorkerMessage();
#ifdef DEBUG
        currentState.printResourcesMessage();
#endif
        return -1;
    }
    currentState.printResourcesMessage();

    //all fine, return 0
    return 0;
}

bool Game::isAnybodyProducing() const
{
    auto producers = currentState.getProducers();

    for (Producer *prod : producers)
    {
        if (prod->isProducing())
        {
            return true;
        }
    }

    auto upgradables = currentState.getUpgradeables();

    for (Upgradable *upgr : upgradables)
    {
        if (upgr->isUpgrading())
        {
            return true;
        }
    }
    return false;
}

void Game::prepareGame() {
    //TODO maxSimTime == 1000 should fit this assignments requirements
    //
    switch (buildOrder.getRace())
    {
        case PROTOSS:
            currentState.addEntity(PROTOSS_PROBE, 6);
            currentState.addEntity(PROTOSS_NEXUS, 1);
            currentState.addCreatedEntity(WarpHelper::Instance());
            break;
        case TERRAN:
            currentState.addEntity(TERRAN_SCV, 6);
            currentState.addEntity(TERRAN_COMMAND_CENTER, 1);
            break;
        case ZERG:
            currentState.addEntity(ZERG_LARVA_HELPER, 1);
            currentState.addEntity(ZERG_DRONE, 6);
            // Only the first hatchery starts with 3 larva
            currentState.addEntity(ZERG_HATCHERY, 1);
            currentState.increaseLarva();
            currentState.increaseLarva();
            currentState.increaseLarva();
            currentState.addEntity(ZERG_OVERLORD, 1);
            break;
        default:
            throw invalid_argument("Unrecognized race");
            break;
    }

    currentState.setMaxSimTime(3000);
    currentState.addMineralsWithFactor(50 * GameState::FACTOR); 
    currentState.consumeEnoughSupply(6 * 1); // Each Worker consumes 1 supply 

    currentState.registerLogger(this);

}

Game::Game(char *file)
    :buildOrder(BuildOrder(file))
{
    prepareGame();
}

Game::Game(std::vector<BuildStep*> buildList) 
    :buildOrder(buildList) {
    prepareGame();
}
