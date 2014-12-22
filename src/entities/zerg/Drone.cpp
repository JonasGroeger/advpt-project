#include "entities/zerg/Drone.hpp"

Drone::Drone()
{
    interfaceBitmask = UPDATABLE_INTERFACE | WORKER_INTERFACE;
    type = ZERG_DRONE;
}

void Drone::update(GameState &state)
{
    if (morphing)
    {
        currentProgress++;
        if (currentProgress >= maxProgress)
        {
            // TODO turn this into a spawning pool
            std::cout << "SPAWNING POOL BUILT" << std::endl;
        }
        return;
    }
    switch (this->getTypeOfWork())
    {
        case TypeOfWork::Minerals:
            state.addMineralsWithFactor(0.7 * GameState::FACTOR);
            break;
        case TypeOfWork::Vespine:
            state.addVespineWithFactor(0.35 * GameState::FACTOR);
            break;
        case TypeOfWork::Idle:
        case TypeOfWork::Producing:
            break;
    }
}

bool Drone::upgradeIfPossible(EntityType type, GameState &state)
{
    if (type == ZERG_SPAWNING_POOL && state.hasEnough(200, 0, 0))
    {
        state.consumeEnough(200, 0, 0);
        state.notifyEntityIsBeingProduced(type);
        currentProgress = 0;
        maxProgress = 65;
        morphing = true;
        return true;
    }
    return false;
}
