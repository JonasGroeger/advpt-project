#include "Units.hpp"

SCV::SCV()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | WORKER_INTERFACE;
    type = TERRAN_SCV;
}

void SCV::update(GameState& state)
{
    Worker* w = static_cast<Worker*>(this);
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
            currentProgress ++;
            if (currentProgress >= maxProgress)
            {
                state.addEntity(product, 1);
                w->setTypeOfWork(TypeOfWork::Idle);
            }
            break;
    }
}

bool SCV::produceEntityIfPossible(EntityType type, GameState& state)
{
    Worker* w = static_cast<Worker*>(this);
    TypeOfWork current = w->getTypeOfWork();
    if(current == TypeOfWork::Producing)
    {
        return false;
    }

    //to check at the end
    unsigned long minerals = 0;
    unsigned long gas = 0;

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
            if(!state.hasEntity(TERRAN_SUPPLY_DEPOT)){
                return false;
            }
            maxProgress = 65;
            minerals = 150;
            break;
        case EntityType::TERRAN_COMMAND_CENTER:
            maxProgress = 100;
            minerals = 400;
            break;
        case EntityType::TERRAN_GHOST_ACADEMY:
            if(!state.hasEntity(TERRAN_BARRACKS)){
                return false;
            }
            maxProgress = 40;
            minerals = 150;
            gas = 50;
            break;
        case EntityType::TERRAN_BUNKER:
            if(!state.hasEntity(TERRAN_BARRACKS)){
                return false;
            }
            maxProgress = 40;
            minerals = 100;
            break;
        case EntityType::TERRAN_FACTORY:
            if(!state.hasEntity(TERRAN_BARRACKS)){
                return false;
            }
            maxProgress = 60;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_ENGINEERING_BAY:
            if(!state.hasEntity(TERRAN_COMMAND_CENTER)){
                return false;
            }
            maxProgress = 30;
            minerals = 125;
            break;
        case EntityType::TERRAN_ARMORY:
            if(!state.hasEntity(TERRAN_FACTORY)){
                return false;
            }
            maxProgress = 35;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_STARPORT:
            if(!state.hasEntity(TERRAN_FACTORY)){
                return false;
            }
            maxProgress = 50;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_FUSION_CORE:
            if(!state.hasEntity(TERRAN_STARPORT)){
                return false;
            }
            maxProgress = 65;
            minerals = 150;
            gas = 150;
            break;
        case EntityType::TERRAN_SENSOR_TOWER:
            if(!state.hasEntity(TERRAN_ENGINEERING_BAY)){
                return false;
            }
            maxProgress = 25;
            minerals = 125;
            gas = 100;
            break;
        case EntityType::TERRAN_MISSILE_TURRET:
            if(!state.hasEntity(TERRAN_ENGINEERING_BAY)){
                return false;
            }
            maxProgress = 25;
            minerals = 100;
            break;
        default:
            return false;
    }

    //we can produce that thing
    if (state.hasEnough(minerals, gas, 0))
    {
        w->setTypeOfWork(TypeOfWork::Producing);
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.notifyEntityIsBeingProduced(type);
        product = type;
        currentProgress = 0;
        return true;
    }
    return false;
}

long SCV::getTimeToFinish()
{
    Worker* w = static_cast<Worker*>(this);

    if (w->getTypeOfWork() == TypeOfWork::Producing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

void SCV::applyChronoBoost()
{
}

bool PlanetaryFortress::produceEntityIfPossible(EntityType type, GameState &state)
{
	return true;
}

long PlanetaryFortress::getTimeToFinish() {
	return 0;
}
void PlanetaryFortress::applyChronoBoost() 
{
}

