#include "entities/zerg/Drone.hpp"

Drone::Drone()
{
    interfaceBitmask = UPDATABLE_INTERFACE | WORKER_INTERFACE | UPGRADABLE_INTERFACE;
    type = ZERG_DRONE;
}

void Drone::update(GameState &state)
{
    if (morphing)
    {
        currentProgress ++;
        if (currentProgress >= maxProgress)
        {
            // TODO turn this into a spawning pool
            state.addEntity(product, 1);
            state.consumeDrone(this);
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
    int minerals = 0, gas = 0, time = 0;
    switch (type)
    {
        case ZERG_SPAWNING_POOL:
            minerals = 200;
            time = 65;
            break;
        case ZERG_SPORE_CRAWLER:
            minerals = 75;
            time = 30;
            break;
        case ZERG_HYDRALISK_DEN:
            minerals = 100;
            gas = 100;
            time = 40;
            break;
        case ZERG_NYDUS_NETWORK:
            minerals = 150;
            gas  = 200;
            time = 50;
            break;
        case ZERG_EVOLUTION_CHAMBER:
            minerals = 75;
            time = 35;
            break;
        case ZERG_INFESTATION_PIT:
            minerals = 100;
            gas = 100;
            time = 50;
            break;
        case ZERG_EXTRACTOR:
            minerals = 25;
            time = 30;
            break;
        case ZERG_SPINE_CRAWLER:
            minerals = 100;
            time = 50;
            break;
        case ZERG_ROACH_WARREN:
            minerals = 180;
            time = 55;
            break;
        case ZERG_BANELING_NEST:
            minerals = 100;
            gas = 50;
            time = 60;
            break;
        case ZERG_SPIRE:
            minerals = 200;
            gas = 200;
            time = 100;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, 0))
    {
        state.consumeEnough(minerals, gas, 0);
        state.notifyEntityIsBeingProduced(type);
        currentProgress = 0;
        maxProgress = time;
        morphing = true;
        product = type;
        return true;
    }
    return false;
}
