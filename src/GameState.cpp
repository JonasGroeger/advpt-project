#include "GameState.hpp"

bool GameState::hasEnoughMinerals(unsigned long amount)
{
    return (amount * MIN_FACTOR) <= minerals;
}
bool GameState::hasEnoughVespine(unsigned long amount)
{
    return (amount * GAS_FACTOR) <= gas;
}
bool GameState::hasEnoughSupply(unsigned long amount)
{
    return amount <= (maximumSupply-usedSupply);
}
bool GameState::hasEntity(EntityType& type, unsigned long amount)
{
    // TODO
    return false;
}

void GameState::consumeEnoughMinerals(unsigned long amount)
{
    minerals -= (amount * MIN_FACTOR);
}
void GameState::consumeEnoughVespine(unsigned long amount)
{
    gas -= (amount * GAS_FACTOR);
}
void GameState::consumeEnoughSupply(unsigned long amount)
{
    usedSupply += amount;
}

void GameState::addMinerals(unsigned long amount)
{
    minerals += (amount * MIN_FACTOR);
}
void GameState::addVespine(unsigned long amount)
{
    gas += (amount * GAS_FACTOR);
}

void GameState::increaseSupply(unsigned long amount)
{
    maximumSupply = (maximumSupply + amount <= 200) ? (maximumSupply + amount) : (200);
}

void GameState::addEntity(EntityType& type, unsigned long amount)
{
    // TODO
}
void GameState::removeEntity(Entity& entity)
{
    // TODO
}
void GameState::changeEntity(Entity& old, Entity& theNew)
{
    // TODO
}

void incrementSimulationTime() {
    this->simulationTime++;
}

int getSimulationTime() {
    return this->simulationTime;
}

vector<EntityType>& GameState::getEntities(EntityType& type)
{
    return this->entityTypes;
}
vector<Upgradable>& GameState::getUpgradeables()
{
    return upgradeables;
}
vector<Updatable>& GameState::getUpdatables()
{
    return updatables;
}
vector<Producer>& GameState::getProducers()
{
    return producers;
}
