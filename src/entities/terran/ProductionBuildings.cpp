#include "entities/terran/ProductionBuildings.hpp"
#include "GameState.hpp"

CommandCenter::CommandCenter()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | UPGRADABLE_INTERFACE;
}

EntityType CommandCenter::getType()
{
    return TERRAN_COMMAND_CENTER;
}

bool CommandCenter::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    switch(type){
        case EntityType::TERRAN_SCV:
            if(state.hasEnough(50, 0, 1))
            {
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                this->state = UPState::PRODUCING;
                currentProgress = 0;
                maxProgress = 17;
                product = type;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

// TODO
bool CommandCenter::upgradeIfPossible(EntityType type, GameState &state)
{
    switch (type)
    {
        case TERRAN_ORBITAL_COMMAND:
            if (state.hasEnoughMinerals(150))
            {
                this->state = UPState::UPGRADING;
                product = type;
                maxProgress = 35;
                state.consumeEnoughMinerals(150);
                return true;
            }
            break;
        case TERRAN_PLANETARY_FORTRESS:
            if (state.hasEnough(150, 150, 0) && state.hasEntity(TERRAN_ENGINEERING_BAY))
            {
                product = type;
                maxProgress = 50;
                state.consumeEnoughMinerals(150);
                state.consumeEnoughVespine(150);
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

Barracks::Barracks()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

EntityType Barracks::getType()
{
    return TERRAN_BARRACKS;
}

bool Barracks::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    switch(type){
        case EntityType::TERRAN_MARINE:
            if(state.hasEnough(50, 0, 1)){
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                this->state = UPState::PRODUCING;
                currentProgress = 0;
                maxProgress = 25;
                product = type;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

// TODO
bool Barracks::upgradeIfPossible(EntityType type, GameState &state)
{
    return false;
}

Factory::Factory()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

EntityType Factory::getType()
{
    return TERRAN_FACTORY;
}

bool Factory::produceEntityIfPossible(EntityType type, GameState &state)
{
    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, supply = 0;
    switch(type)
    {
        case EntityType::TERRAN_HELLION:
            minerals = 100;
            supply = 2;
            maxProgress = 30;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        this->state = UPState::PRODUCING;
        currentProgress = 0;
        product = type;
        return true;
    }

    return false;
}

// TODO
bool Factory::upgradeIfPossible(EntityType type, GameState &state)
{
    return false;
}

Starport::Starport()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

EntityType Starport::getType()
{
    return TERRAN_STARPORT;
}

bool Starport::produceEntityIfPossible(EntityType type, GameState &state)
{
    if(isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, supply = 0;
    switch(type)
    {
        case EntityType::TERRAN_MEDIVAC:
            minerals = 100;
            gas = 100;
            supply = 2;
            maxProgress = 42;
            break;
        case EntityType::TERRAN_VIKING:
            minerals = 150;
            gas = 75;
            supply = 2;
            maxProgress = 42;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        this->state = UPState::PRODUCING;
        currentProgress = 0;
        product = type;
        return true;
    }

    return false;
}

// TODO
bool Starport::upgradeIfPossible(EntityType type, GameState &state)
{
    return false;
}