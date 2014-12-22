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

bool SimpleProducer::isProducing()
{
    return state == UPState::PRODUCING;
}

void SimpleProducer::applyChronoBoost()
{
    // TODO implement
}
