#include "entities/UpgradableProducer.hpp"
#include "GameState.hpp"

bool UpgradableProducer::isBusy()
{
    return state == UPState::PRODUCING || state == UPState::UPGRADING;
}

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
                // TODO change this to changeEntity when changeEntity is implemented
                gameState.addEntity(product, 1);
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
