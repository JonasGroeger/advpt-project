#include "GameState.hpp"

bool GameState::hasEnoughMinerals(unsigned long amount)
{
    return amount * MIN_FACTOR <= minerals;
}
bool GameState::hasEnoughVespine(unsigned long amount)
{
    return amount * GAS_FACTOR <= gas;
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

unsigned long GameState::getMinerals()
{
        return minerals;
}

void GameState::increaseSupply(unsigned long amount)
{
    maximumSupply = (maximumSupply + amount <= 200) ? (maximumSupply + amount) : (200);
}

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
        }
        else if (type == TERRAN_SCV)
        {
            SCV *new_unit = new SCV();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            updatables.push_back(dynamic_cast<Updatable*> (new_unit));
            producers.push_back(dynamic_cast<Producer*> (new_unit));
        }
        else if (type == TERRAN_BARRACKS)
        {
            Barracks *new_unit = new Barracks();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
            updatables.push_back(dynamic_cast<Updatable*> (new_unit));
            producers.push_back(dynamic_cast<Producer*> (new_unit));
        }
        else if (type == TERRAN_SUPPLY_DEPOT)
        {
            SupplyDepot *new_unit = new SupplyDepot();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
        }
        else if (type == TERRAN_MARINE)
        {
            Marine *new_unit = new Marine();
            entities.push_back(dynamic_cast<Entity*> (new_unit));
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
