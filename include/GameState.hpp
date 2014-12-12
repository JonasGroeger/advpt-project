#pragma once

#include <vector>

#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "Producer.hpp"
#include "Entity.hpp"

using std::vector;

class Upgradable;
class Updatable;

class GameState
{
// TODO this should be private but it won't compile anymore
public:
    vector<EntityType> entityTypes;
    vector<Upgradable> upgradeables;
    vector<Updatable> updatables;
    vector<Producer> producers;

public:
    bool hasEnoughMinerals(long amount);
    bool hasEnoughVespine(long amount);
    bool hasEnoughSupply(long amount);
    bool hasEntity(EntityType& type, long amount);

    void consumeEnoughMinerals(long amount);
    void consumeEnoughVespine(long amount);
    void consumeEnoughSupply(long amount);

    void addMinerals(long amount);
    void addVespine(long amount);

    void addEntity(EntityType& type, long amount);
    void removeEntity(Entity& entity);
    void changeEntity(Entity& old, Entity& theNew);

    vector<EntityType>& getEntities(EntityType& type);
    vector<Upgradable>& getUpgradeables();
    vector<Updatable>& getUpdatables();
    vector<Producer>& getProducers();
};
