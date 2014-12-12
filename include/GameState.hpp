#pragma once

#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "Producer.hpp"

class GameState
{
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

    std::vector<EntityType>& getEntities(EntityType& type);
    std::vector<Upgradable>& getUpgradeables();
    std::vector<Updatable>& getUpdatables();
    std::vector<Producer>& getProducers();
};
