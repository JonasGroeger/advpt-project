#pragma once

#include <vector>
#include <bitset>

#include "Units.hpp"
#include "entities/Producer.hpp"
#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Worker.hpp"
#include "entities/terran/ProductionBuildings.hpp"

class Game;

class Updatable;
class Upgradable;

using std::vector;

class GameState
{
private:
    // Ressources stored multiplied by FACTOR
    unsigned long minerals = 0, gas = 0;

    /*
     * This describes how many workers could harvest vespene currently
     * It is increased whenever a Refinery is built
     */
    unsigned long vespeneSlots = 0;

    /*
     * Supply is a ressource that gives a maximum to how many units a player can control.
     * Some units and buildings increase the maximum supply this acts like a capacity.
     * Note that maximumSupply can never be increased over 200.
     * Most units use a certain amount of supply. This increases the usedSupply.
     * So maximumSupply - usedSupply gives the available amount of supply.
     */
    unsigned long usedSupply = 0, maximumSupply = 0;

    unsigned int simulationTime = 0;
    unsigned int maxTime = 0;

    vector<EntityType> entityTypes;
    vector<Upgradable*> upgradeables;
    vector<Updatable*> updatables;
    vector<Producer*> producers;
    vector<Worker*> workers;
    vector<Entity*> entities;

    Game *logger = nullptr;
    void addEntityToVectors(Entity* entity);


// TODO this should be private but it won't compile anymore
public:
    // minerals and gas is stored multiplied by MIN_FACTOR and GAS_FACTOR respectively
    static const unsigned long FACTOR = 100;

    //This bitset contains a setted bit if this entity is already created by us
    //it is initialized by 50 to be sure to have enough capacity for our terran entities
    std::bitset<50> constructedBitset = 0;
    //see above bitset comment - except this bitset holds entities not finished but in construction
    std::bitset<50> entitiesInConstruction = 0;
    
    //All methods here receive the real value they need as we have no floats here
    bool hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply);
    bool hasEnoughMinerals(unsigned long amount) const;
    bool hasEnoughVespine(unsigned long amount) const;
    bool hasEnoughSupply(unsigned long amount) const;

    //This method checks if type is existing/already created in our current state
    bool hasEntity(EntityType type) const;

    //This method checks if type is currently being produced
    bool hasEntityInProduction(EntityType type) const;

    // Use actual amount
    void consumeEnoughMinerals(unsigned long amount);
    void consumeEnoughVespine(unsigned long amount);
    void consumeEnoughSupply(unsigned long amount);

    // Use amount * FACTOR
    void addMineralsWithFactor(unsigned long amount);
    void addVespineWithFactor(unsigned long amount);
    void increaseSupply(unsigned long amount);

    unsigned long getMinerals() const;
    unsigned long getVespine() const;
    unsigned long getUsedSupply() const;
    unsigned long getAvailableSupply() const;

    void notifyEntityIsBeingProduced(EntityType type);
    void setAvailableEntityType(EntityType type);

    //this method sets the value of type to 1 in the entitiesBeingProduced bitset
    void addEntity(EntityType type, unsigned long amount);
    void addCreatedEntity(Entity* entity);

    void removeEntity(Entity& entity); // TODO implement
    void changeEntity(Entity& old, Entity& theNew); // TODO implement

    //returns true if simTime >= maxTime, false otherwise
    void setMaxSimTime(int time);
    bool maxSimTimeReached() const;
    int getSimulationTime() const;
    void incrementSimulationTime();

    bool hasOpenVespeneSlot();

    //contains all created entities
    const vector<EntityType>& getEntities(EntityType& type) const;
	
    //following three vectors only contain pointers to specific elements of the above one
    //contains references to the getEntities vector (views)
    const vector<Upgradable*>& getUpgradeables() const;
    const vector<Updatable*>& getUpdatables() const;
    const vector<Producer*>& getProducers() const;
    const vector<Worker*>& getWorkers() const;

    void registerLogger(Game *newLogger);
    void unregisterLogger();
};
