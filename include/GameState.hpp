#pragma once

#include <vector>
#include <bitset>

#include "Units.hpp"
#include "Producer.hpp"
#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Worker.hpp"

class Updatable;
class Upgradable;

using std::vector;
class GameState
{

private:
    unsigned long minerals, gas, supply;
    /*
     * Supply is a ressource that gives a maximum to how many units a player can control.
     * Some units and buildings increase the maximum supply this acts like a capacity.
     * Note that maximumSupply can never be increased over 200.
     * Most units use a certain amount of supply. This increases the usedSupply.
     * So maximumSupply - usedSupply gives the available amount of supply.
     */
    unsigned long usedSupply, maximumSupply;

    unsigned int simulationTime;
    unsigned int maxTime = 0;

// TODO this should be private but it won't compile anymore
public:
    GameState(unsigned int maxSimTime, unsigned long initialGas, unsigned long initialSupply, unsigned long initialsMinerals){
        gas = initialGas * FACTOR;
        minerals = initialsMinerals * FACTOR;
        supply = initialSupply * FACTOR;
        maxTime = maxSimTime;
        simulationTime = 0;
    }
    
    vector<EntityType> entityTypes;
    vector<Upgradable*> upgradeables;
    vector<Updatable*> updatables;
    vector<Producer*> producers;
    vector<Worker*> workers;
    vector<Entity*> entities;

    // minerals and gas is stored multiplied by MIN_FACTOR and GAS_FACTOR respectively
    static const unsigned long FACTOR = 100;

    //This bitset contains a setted bit if this entity is already created by us
    //it is initialized by 50 to be sure to have enough capacity for our terran entities
    std::bitset<50> constructedBitset = 0;
    //see above bitset comment - except this bitset holds entities not finished but in construction
    std::bitset<50> entitiesInConstruction = 0;
    
    //All methods here receive the real value they need as we have no floats here
    bool hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply);
    bool hasEnoughMinerals(unsigned long amount);
    bool hasEnoughVespine(unsigned long amount);
    bool hasEnoughSupply(unsigned long amount);

    //This method checks if type is existing/already created in our current state
    bool hasEntity(EntityType type);
    //This method checks if type is currently being produced
    bool hasEntityInProduction(EntityType type);

    // Use actual amount
    void consumeEnoughMinerals(unsigned long amount);
    void consumeEnoughVespine(unsigned long amount);
    void consumeEnoughSupply(unsigned long amount);

    // Use amount * 100
    void addMineralsWithFactor(unsigned long amount);
    void addVespineWithFactor(unsigned long amount);
    void increaseSupply(unsigned long amount);

    unsigned long getMinerals();

    void addEntity(EntityType type, unsigned long amount);
    //this method sets the value of type to 1 in the entitiesBeingProduced bitset
    void notifyEntityIsBeingProduced(EntityType type);
    void removeEntity(Entity& entity);
    void changeEntity(Entity& old, Entity& theNew);

    //returns true if simTime >= maxTime, false otherwise
    bool maxSimTimeReached();
    int getSimulationTime();
    void incrementSimulationTime();

	//contains all created entities
    vector<EntityType>& getEntities(EntityType& type);
	
	//following three vectors only contain pointers to specific elements of the above one
	//contains references to the getEntities vector (views)
    vector<Upgradable*>& getUpgradeables();
    vector<Updatable*>& getUpdatables();
    vector<Producer*>& getProducers();
    vector<Worker*>& getWorkers();
};
