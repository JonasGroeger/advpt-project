#pragma once

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
    void changeEntity(Entity& old: Entity& theNew);

    std::vector<EntityType>& getEntities(EntityType& type);
    std::vector<Upgradables>& getUpgradeables();
    std::vector<Updatables>& getUpdatables();
    std::vector<Producer>& getProducers();
};
