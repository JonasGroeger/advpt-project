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
            }
        default:
            return;
    }
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
