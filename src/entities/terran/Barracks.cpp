#include "entities/terran/Barracks.hpp"
#include "GameState.hpp"

Barracks::Barracks()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

EntityType Barracks::getType()
{
    return TERRAN_BARRACKS;
}

void Barracks::update(GameState& state)
{
    if (isProducing)
    {
        if (currentProgress >= maxProgress)
        {
            state.addEntity(TERRAN_MARINE, 1);
            isProducing = false;
            currentProgress = 0;
        }
        currentProgress ++;
    }
}

bool Barracks::produceEntityIfPossible(EntityType type, GameState& state)
{
    switch(type){
        case EntityType::TERRAN_MARINE:
            if(state.hasEnoughMinerals(50)){
                isProducing = true;
                state.consumeEnoughMinerals(50);
                state.notifyEntityIsBeingProduced(type);
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

void Barracks::applyChronoBoost()
{
}

long Barracks::getTimeToFinish()
{
    if (isProducing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}