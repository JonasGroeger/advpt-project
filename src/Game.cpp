#include "Game.hpp"

bool Game::executeBuildStep(BuildStep* step)
{
    std::cout << "EntityType: " << BuildStep::entityTypeToString[step->getWhich()] << std::endl;
    /*
    switch (step->getType())
    {
        case BuildStepType::UPGRADE:
            std::cout << "THAT WAS AN UPGRADE!!!" << std::endl;
            break;
        case BuildStepType::PRODUCE:
            std::cout << "THAT WAS AN PRODUCTION!!!" << std::endl;
            // TODO
            break;
        case BuildStepType::CONSTRUCT:
            std::cout << "THAT WAS AN CONSTRUCTION!!!" << std::endl;
            // TODO
            break;
        case BuildStepType::CHRONO_BOOST:
            std::cout << "THAT WAS AN CHRONO_BOOST!!!" << std::endl;
            // TODO
            break;
    }*/

    return true;
}

void Game::loop()
{
    // TODO add a maximum number of steps
    while (true)
    {
        // We update each updateable
        auto updatables = currentState.getUpdatables();

        std::for_each(updatables.begin(), updatables.end(), 
                [this] (Updatable& updt) { updt.update(this->currentState); }
        );

        // We try to execute the next BuildStep
        BuildStep* nextStep = buildOrder.getNextStep();

        if (nextStep == nullptr)
        {
            return;
        } 
        else if (executeBuildStep(nextStep))
        {
            buildOrder.advance();
        }
    }
}

bool Game::isFinished()
{

}

GameState& Game::getFinalState()
{

}

// TODO pass a BuildOrder and initiliaze GameState
Game::Game(char *file)
    : buildOrder(BuildOrder(file))
{
}
