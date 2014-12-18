#include "Units.hpp"
#include <iostream>

EntityType SCV::getType()
{
    return TERRAN_SCV;
}

void SCV::update(GameState& state)
{
    if (isProducing)
    {
        if (currentProgress >= maxProgress)
        {
            state.addEntity(product, 1);
            isProducing = false;
            currentProgress = 0;
        }
        currentProgress ++;
    }
    else
    {
        state.addMinerals(7);
    }
}

bool SCV::canProduce(EntityType type, GameState& state)
{
    if (type == TERRAN_SUPPLY_DEPOT)
    {
        return (!isProducing) && state.hasEnoughMinerals(100); // TODO we should use isConstructable
    }
    else if (type == TERRAN_BARRACKS)
    {
        return (!isProducing) && state.hasEnoughMinerals(150); // TODO we should use isConstructable
    }
    return false;
}
/* 
 * Only call if canProduce returned true!!!
 */
void SCV::produce(EntityType type, GameState& state)
{
    // TODO different build times
    if (type == TERRAN_SUPPLY_DEPOT)
    {
        state.consumeEnoughMinerals(100);
        maxProgress = 30;
    }
    else if (type == TERRAN_BARRACKS)
    {
        state.consumeEnoughMinerals(150);
        maxProgress = 65;
    }
    isProducing = true;
    currentProgress = 0;
    maxProgress = 65;
    product = type;
}

long SCV::getTimeToFinish()
{
    if (isProducing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}
void SCV::applyChronoBoost()
{
}

EntityType CommandCenter::getType()
{
    return TERRAN_COMMAND_CENTER;
}

void CommandCenter::update(GameState& state)
{
    if (isProducing)
    {
        if (currentProgress >= maxProgress)
        {
            state.addEntity(TERRAN_SCV, 1);
            isProducing = false;
            currentProgress = 0;
        }
        currentProgress ++;
    }
}

bool CommandCenter::canProduce(EntityType type, GameState& state)
{
    if (type == TERRAN_SCV)
    {
        return (!isProducing) && state.hasEnoughMinerals(50);
    }
    return false;
}
void CommandCenter::produce(EntityType type, GameState& state)
{
    if (type == TERRAN_SCV)
    {
        state.consumeEnoughMinerals(50);
        isProducing = true;
        currentProgress = 0;
    }
}

long CommandCenter::getTimeToFinish()
{
    if (isProducing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}
void CommandCenter::applyChronoBoost()
{
}

EntityType SupplyDepot::getType()
{
    return TERRAN_SUPPLY_DEPOT;
}

bool SupplyDepot::isConstructable(GameState& state)
{
    return state.hasEnoughMinerals(100);
}
void SupplyDepot::construct(GameState& state)
{
    state.consumeEnoughMinerals(100);
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

bool Barracks::canProduce(EntityType type, GameState& state)
{
    std::cerr << "Barracks: canProduce" << std::endl;
    if (type == TERRAN_MARINE)
    {
        return state.hasEnoughMinerals(50);
    }
    return false;
}
void Barracks::produce(EntityType type, GameState& state)
{
    if (type == TERRAN_MARINE)
    {
        state.consumeEnoughMinerals(50);
        isProducing = true;
        currentProgress = 0;
    }
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

EntityType Marine::getType()
{
    return TERRAN_MARINE;
}
