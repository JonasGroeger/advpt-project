#include "entities/terran/SCV.hpp"
#include "GameState.hpp"

EntityType SCV::getType()
{
    return TERRAN_SCV;
}

SCV::SCV()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | WORKER_INTERFACE;
}

void SCV::update(GameState& state)
{
    Worker* w = dynamic_cast<Worker*>(this);
    switch (w->getTypeOfWork()){
        case TypeOfWork::Idle:
            break;
        case TypeOfWork::Minerals:
            state.addMineralsWithFactor(0.7 * GameState::FACTOR);
            break;
        case TypeOfWork::Vespine:
            state.addVespineWithFactor(0.35 * GameState::FACTOR);
            break;
        case TypeOfWork::Producing:
            if (currentProgress >= maxProgress)
            {
                state.addEntity(product, 1);
                isProducing = false;
                w->setTypeOfWork(TypeOfWork::Minerals);
                currentProgress = 0;
            }
            currentProgress ++;
            break;
    }
}

bool SCV::produceEntityIfPossible(EntityType type, GameState& state)
{
    Worker* w = dynamic_cast<Worker*>(this);
    TypeOfWork current = w->getTypeOfWork();
    if(current == TypeOfWork::Producing)
    {
        return false;
    }

    //to check at the end
    unsigned long minerals = 0;
    unsigned long gas = 0;
    unsigned long supply = 0;

    switch(type)
    {
        case EntityType::TERRAN_SUPPLY_DEPOT:
            maxProgress = 30;
            minerals = 100;
            break;
        case EntityType::TERRAN_REFINERY:
            maxProgress = 30;
            minerals = 75;
            break;
        case EntityType::TERRAN_BARRACKS:
            maxProgress = 65;
            minerals = 150;
            break;
        case EntityType::TERRAN_COMMAND_CENTER:
            maxProgress = 100;
            minerals = 400;
            break;
        case EntityType::TERRAN_GHOST_ACADEMY:
            maxProgress = 40;
            minerals = 150;
            gas = 50;
            break;
        case EntityType::TERRAN_BUNKER:
            maxProgress = 40;
            minerals = 100;
            break;
        case EntityType::TERRAN_FACTORY:
            maxProgress = 60;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_ENGINEERING_BAY:
            maxProgress = 30;
            minerals = 125;
            break;
        case EntityType::TERRAN_ARMORY:
            maxProgress = 35;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_STARPORT:
            maxProgress = 50;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_FUSION_CORE:
            maxProgress = 65;
            minerals = 150;
            gas = 150;
            break;
        case EntityType::TERRAN_SENSOR_TOWER:
            maxProgress = 25;
            minerals = 125;
            gas = 100;
            break;
        case EntityType::TERRAN_MISSILE_TURRET:
            maxProgress = 25;
            minerals = 100;
            break;
        default:
            return false;
    }

    //we can produce that thing
    if (state.hasEnough(minerals, gas, supply))
    {
        w->setTypeOfWork(TypeOfWork::Producing);
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        state.notifyEntityIsBeingProduced(type);
        product = type;
        return true;
    }

    // We cant produce.
    return false;
}


long SCV::getTimeToFinish()
{
    Worker* w = dynamic_cast<Worker*>(this);

    if (w->getTypeOfWork() == TypeOfWork::Producing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

void SCV::applyChronoBoost()
{
}