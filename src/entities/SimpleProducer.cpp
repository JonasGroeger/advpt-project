#include "entities/SimpleProducer.hpp"
#include "GameState.hpp"
#include <iostream>

void SimpleProducer::update(GameState& state)
{
    if (isProducing)
    {
        currentProgress ++;

        if (currentProgress >= maxProgress)
        {
            state.addEntity(product, 1);
            isProducing = false;
        }
    }
}

long SimpleProducer::getTimeToFinish()
{
    return isProducing ? maxProgress - currentProgress : 0;
}

void SimpleProducer::applyChronoBoost()
{
    // TODO implement
}
