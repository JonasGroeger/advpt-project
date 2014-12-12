#include "Game.hpp"

bool Game::executeBuildStep(BuildStep* step)
{
    std::cout << "Executed: " << step->getName() << std::endl;
    return true;
}

void Game::loop()
{
    // TODO add a maximum number of steps
    while (true)
    {
        // TODO this won't compile and i don't really know why yet
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
