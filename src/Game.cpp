#include "Game.hpp"
#include "Debug.hpp"

unsigned long Game::getFitnessPush(BuildOrder &order)
{
    Game *game = new Game(order);
    unsigned int runTime = game->loop();
    delete game;
    
    return runTime;
}

bool Game::executeBuildStep(BuildStep *step)
{
    // Temporarly deactivated because anoying
    //LOG_DEBUG("Time=" << currentState.getSimulationTime() << ", Minerals=" << currentState.getMinerals());

    BuildStepType buildStepType = step->getBuildStepType();

    /* Because we assign variables inside the switch (auto upgradables, auto producers, ...) we need to
       scope the cases like so: "case x: {...}". See http://stackoverflow.com/q/7562199 */
    switch (buildStepType)
    {

        case BuildStepType::UPGRADE:
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
            break;
        }

        case BuildStepType::PRODUCE:
        {
            auto producers = currentState.getProducers();
            for (auto it = producers.begin(); it != producers.end(); it++)
            {
                Producer *prod = *it;

                //TODO fix produce interface

                if (prod->produceEntityIfPossible(step->getEntityType(), currentState))
                {
                    currentState.printBuildStartMessage(step->getEntityType());
                    return true;
                }
            }
            break;
        }
        case BuildStepType::CHRONO_BOOST:
        {
            std::cerr << "chrono boosting a: ";
            // TODO
            break;
        }
    }

    return false;
}

int Game::loop()
{
    while ((!buildOrder.isDone() || isAnybodyProducing())
            && !currentState.maxSimTimeReached())
    {
        bool somethingHappened = false;

        /*
         * We update before executing any build steps so any Producer
         * that finishes this tick is cleared to produce something new
         */
        auto updatables = currentState.getUpdatables();
        std::for_each(updatables.begin(), updatables.end(),
                [this](Updatable *updt) {
                    updt->update(this->currentState);
                }
        );

        // As long as there are still steps left
        while (!buildOrder.isDone())
        {
            BuildStep *nextStep = buildOrder.getNextStep();

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

        if (somethingHappened)
        {
#ifdef DEBUG
            currentState.printResourcesMessage();
#endif
            currentState.printWorkerMessage();
        }

        currentState.incrementSimulationTime();
    }

    if (currentState.maxSimTimeReached() && !buildOrder.isDone())
    {
        //buildlist did not succeed so return non zero and print error message
        std::cerr << "Reached maximum Time - aborting..." << std::endl;
        currentState.printWorkerMessage();
#ifdef DEBUG
        currentState.printResourcesMessage();
#endif
        return currentState.getSimulationTime();
    }
    currentState.printResourcesMessage();

    //all fine, return 0
    return currentState.getSimulationTime();
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

// TODO reset stuff before
void Game::prepareGame()
{
    //TODO maxSimTime == 1000 should fit this assignments requirements
    EntityType race = buildOrder.getRace();
    switch (race)
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
            cerr << race << endl;
            throw invalid_argument("Unrecognized race");
            break;
    }

    //currentState.setMaxSimTime(10*60);
    currentState.setMaxSimTime(30000);
    currentState.addMineralsWithFactor(50 * GameState::FACTOR);
    currentState.consumeEnoughSupply(6 * 1); // Each Worker consumes 1 supply 

    currentState.registerLogger(this);
}

Game::Game(char *file)
        : buildOrder(file)
{
    prepareGame();
}

Game::Game(std::vector<BuildStep *> buildList)
        : buildOrder(buildList)
{
    prepareGame();
}

Game::Game(BuildOrder& order)
    : buildOrder(order)
{
    prepareGame();
}
