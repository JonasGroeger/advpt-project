#include "entities/SimpleProducer.hpp"
#include "GameState.hpp"

void SimpleProducer::update(GameState& state)
{
    switch(this->state)
    {
        case UPState::PRODUCING:
            currentProgress ++;

            if (currentProgress >= maxProgress)
            {
                state.addEntity(product, 1);
                this->state = UPState::IDLE;
            }
            break;
        default:
            return;
    }
}

long SimpleProducer::getTimeToFinish()
{
    return (state == UPState::PRODUCING)
            ? maxProgress - currentProgress
            : 0;
}

void SimpleProducer::applyChronoBoost()
{
    // TODO implement
}