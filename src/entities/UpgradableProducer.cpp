#include <iostream>
#include <BuildStep.hpp>
#include <iomanip>
#include "entities/UpgradableProducer.hpp"
#include "GameState.hpp"

void UpgradableProducer::update(GameState& gameState)
{
    switch(this->state)
    {
        case UPState::PRODUCING:
            currentProgress ++;

            if (currentProgress >= maxProgress)
            {
                gameState.addEntity(product, 1);
                this->state = UPState::IDLE;
            }
        break;
        case UPState::UPGRADING:
            currentProgress ++;

            if (currentProgress >= maxProgress)
            {
                //just switch our own entitytype and notify gamestate of the new type
                Entity* ourEntity = dynamic_cast<Entity*>(this);
                ourEntity->setType(product);
                gameState.setAvailableEntityType(product);
                //gameState.addEntity(product, 1);
                this->state = UPState::IDLE;
                printBuildEndMessage(product, gameState.getSimulationTime());
            }
        default:
            return;
    }
}

void UpgradableProducer::printMessageProlog(unsigned int time) const
{
    std::cout << std::left;
    std::cout << std::setw(5);
    std::cout << time;
    std::cout << std::setw(14);
}

void UpgradableProducer::printBuildStartMessage(EntityType type, unsigned int time) const
{
    printMessageProlog(time);
    std::cout << "build-start";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
}

void UpgradableProducer::printBuildEndMessage(EntityType type, unsigned int time) const
{
    printMessageProlog(time);
    std::cout << "build-end";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
}

long UpgradableProducer::getTimeToFinish()
{
    if (isBusy())
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

void UpgradableProducer::applyChronoBoost()
{
    // TODO implement
}
