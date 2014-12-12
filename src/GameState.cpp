#include "GameState.hpp"

bool GameState::hasEnoughMinerals(long amount)
{
    return false;
}
bool GameState::hasEnoughVespine(long amount)
{
    return false;
}
bool GameState::hasEnoughSupply(long amount)
{
    return false;
}
bool GameState::hasEntity(EntityType& type, long amount)
{
    return false;
}

void GameState::consumeEnoughMinerals(long amount)
{

}
void GameState::consumeEnoughVespine(long amount)
{

}
void GameState::consumeEnoughSupply(long amount)
{

}

void GameState::addMinerals(long amount)
{

}
void GameState::addVespine(long amount)
{

}

void GameState::addEntity(EntityType& type, long amount)
{

}
void GameState::removeEntity(Entity& entity)
{

}
void GameState::changeEntity(Entity& old, Entity& theNew)
{

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
