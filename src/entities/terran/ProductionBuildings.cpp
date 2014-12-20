#include "entities/terran/ProductionBuildings.hpp"
#include "GameState.hpp"
#include <iostream>

CommandCenter::CommandCenter()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

EntityType CommandCenter::getType()
{
    return TERRAN_COMMAND_CENTER;
}

bool CommandCenter::produceEntityIfPossible(EntityType type, GameState& state)
{
    if(isProducing){
        return false;
    }

    switch(type){
        case EntityType::TERRAN_SCV:
            if(state.hasEnough(50, 0, 1)){
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                isProducing = true;
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
    if (isProducing)
    {
        return false;
    }

    switch(type){
        case EntityType::TERRAN_MARINE:
            if(state.hasEnough(50, 0, 1)){
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                isProducing = true;
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
    if(isProducing)
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
        isProducing = true;
        currentProgress = 0;
        product = type;
        return true;
    }

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
    if(isProducing)
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
        isProducing = true;
        currentProgress = 0;
        product = type;
        return true;
    }

    return false;
}
