#include "Units.hpp"
#include <iostream>
#include <Python/Python.h>

EntityType SCV::getType()
{
    return TERRAN_SCV;
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

bool SCV::canProduce(EntityType type, GameState& state)
{
    Worker* w = dynamic_cast<Worker*>(this);

    if(w->getTypeOfWork() == TypeOfWork::Producing){
        return false;
    }

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
    Worker* w = dynamic_cast<Worker*>(this);
    w->setTypeOfWork(TypeOfWork::Producing);

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

EntityType Factory::getType()
{
    return TERRAN_FACTORY;
}

EntityType Refinery::getType()
{
    return TERRAN_REFINERY;
}

EntityType Hellion::getType()
{
    return TERRAN_HELLION;
}

EntityType Starport::getType()
{
    return TERRAN_STARPORT;
}

EntityType BattleCruiser::getType()
{
    return TERRAN_BATTLECRUISER;
}

Starport::Starport()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

Factory::Factory()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

/* TODO: IMPLEMENT the functions below, as they are only dummies */

bool Starport::canProduce(EntityType type, GameState &state)
{
    // TODO: Implement
    return Producer::canProduce(type, state);
}

void Starport::produce(EntityType type, GameState &state)
{
    // TODO: Implement
    Producer::produce(type, state);
}

long Starport::getTimeToFinish()
{
    // TODO: Implement
    return Producer::getTimeToFinish();
}

void Starport::applyChronoBoost()
{
    // TODO: Implement
    Producer::applyChronoBoost();
}

bool Factory::canProduce(EntityType type, GameState &state)
{
    // TODO: Implement
    return Producer::canProduce(type, state);
}

void Factory::update(GameState &state)
{
    // TODO: Implement
    Updatable::update(state);
}

void Factory::applyChronoBoost()
{
    // TODO: Implement
    Producer::applyChronoBoost();
}

long Factory::getTimeToFinish()
{
    // TODO: Implement
    return Producer::getTimeToFinish();
}

void Factory::produce(EntityType type, GameState &state)
{
    // TODO: Implement
    Producer::produce(type, state);
}

SCV::SCV()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | WORKER_INTERFACE;
}

CommandCenter::CommandCenter()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

Barracks::Barracks()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}
