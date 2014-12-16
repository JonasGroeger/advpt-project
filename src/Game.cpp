#include "Game.hpp"


bool Game::executeBuildStep(BuildStep* step)
{
    //TODO remove these hardcoded costs and get them from current entitytype
	int mineral = 2;
	int gas = 1;
	int supply = 3;
	bool success = false;
    //success defines if we have enough of the needed costs to execute this step
    success = currentState.hasEnoughMinerals(1)
        && currentState.hasEnoughVespine(1)
        && currentState.hasEnoughSupply(1);

    if(!success){
        return false;
    }
    switch (step->getType())
    {
        case BuildStepType::UPGRADE:
            std::cout << "upgrading a: ";
            break;
        case BuildStepType::PRODUCE:
            std::cout << "producing a: ";
			//if(gamestate.haseenough....(mineral, gas, supply) && //nebenbedingungen (mehr als eine instanz erlaubt etc...)) {
			//	getEntities.push_back(createdOne)
			//	GameState
				success =true;
			//}
            break;
        case BuildStepType::CONSTRUCT:
            std::cout << "constructing a: ";
            // TODO
            break;
        case BuildStepType::CHRONO_BOOST:
            std::cout << "chrono boosting a: ";
            // TODO
            break;
    }
    std::cout << BuildStep::entityTypeToString[step->getWhich()] << std::endl;

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
        //if we have enough to execute current buildstep, we go further until it fails
        while(executeBuildStep(nextStep)){
            //TODO fix duplicate code here, see above
            nextStep = buildOrder.getNextStep();
            if(nextStep == nullptr){
                return;
            }
			buildOrder.advance();
		}
		//count simulation time one step up
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
