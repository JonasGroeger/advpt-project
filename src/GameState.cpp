#include <ostream>
#include <iostream>
#include "GameState.hpp"

bool GameState::hasEnoughMinerals(unsigned long amount)
{
    return amount * FACTOR <= minerals;
}
bool GameState::hasEnoughVespine(unsigned long amount)
{
    return amount * FACTOR <= gas;
}
bool GameState::hasEnoughSupply(unsigned long amount)
{
    return amount * FACTOR <= (maximumSupply-usedSupply);
}
bool GameState::hasEntity(EntityType type)
{
    //TODO do we really need amount as a param here?
    return constructedBitset.test(type);
}

void GameState::consumeEnoughMinerals(unsigned long amount)
{
    minerals -= (amount * FACTOR);
}
void GameState::consumeEnoughVespine(unsigned long amount)
{
    gas -= (amount * FACTOR);
}
void GameState::consumeEnoughSupply(unsigned long amount)
{
    usedSupply += amount * FACTOR;
}

void GameState::addMineralsWithFactor(unsigned long amount)
{
    minerals += amount;
}
void GameState::addVespineWithFactor(unsigned long amount)
{
    gas += amount;
}

unsigned long GameState::getMinerals()
{
    //TODO we have rounded values here maybe use float?
        return minerals/FACTOR;
}

void GameState::increaseSupply(unsigned long amount)
{
    maximumSupply = (maximumSupply + amount <= 200) ? (maximumSupply + amount) : (200);
}

//map<EntityType, Entity, BitMask> = {TERRAN_BARRACKS, dynamic_cast<Entity>(new Barracks()), ENTITY | UPDATABLE | PRODUCER}

void GameState::addEntity(EntityType type, unsigned long amount)
{
    for (unsigned long i = 0; i < amount; i++)
    {
        if (type == TERRAN_COMMAND_CENTER)
        {
            CommandCenter *new_unit = new CommandCenter();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            updatables.push_back(dynamic_cast<Updatable*> (new_unit));
            producers.push_back(dynamic_cast<Producer*> (new_unit));
            //add this entity to our bitset
            constructedBitset.set(TERRAN_COMMAND_CENTER);
        }
        else if (type == TERRAN_SCV)
        {
            SCV *new_unit = new SCV();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            updatables.push_back(dynamic_cast<Updatable*> (new_unit));
            producers.push_back(dynamic_cast<Producer*> (new_unit));
            constructedBitset.set(TERRAN_SCV);

        }
        else if (type == TERRAN_BARRACKS)
        {
            Barracks *new_unit = new Barracks();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            updatables.push_back(dynamic_cast<Updatable*> (new_unit));
            producers.push_back(dynamic_cast<Producer*> (new_unit));
            constructedBitset.set(TERRAN_BARRACKS);
        }
        else if (type == TERRAN_SUPPLY_DEPOT)
        {
            SupplyDepot *new_unit = new SupplyDepot();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            constructedBitset.set(TERRAN_SUPPLY_DEPOT);
        }
        else if (type == TERRAN_MARINE)
        {
            Marine *new_unit = new Marine();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            constructedBitset.set(TERRAN_MARINE);
        }
    }
}
void GameState::removeEntity(Entity& entity)
{
    // TODO
}
void GameState::changeEntity(Entity& old, Entity& theNew)
{
    // TODO
}

bool GameState::maxSimTimeReached(){
    return (simulationTime >= maxTime);
}

void GameState::incrementSimulationTime() {
    this->simulationTime++;
}

int GameState::getSimulationTime() {
    return this->simulationTime;
}

vector<EntityType>& GameState::getEntities(EntityType& type)
{
    return this->entityTypes;
}
vector<Upgradable*>& GameState::getUpgradeables()
{
    return upgradeables;
}
vector<Updatable*>& GameState::getUpdatables()
{
    return updatables;
}
vector<Producer*>& GameState::getProducers()
{
    return producers;
}

bool GameState::hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply)
{
    return hasEnoughMinerals(minerals) && hasEnoughVespine(vespine) && hasEnoughSupply(supply);
}
