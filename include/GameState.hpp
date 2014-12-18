#pragma once

#include <vector>
#include <bitset>

#include "Units.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "Producer.hpp"
#include "Entity.hpp"
#include "EntityType.hpp"


using std::vector;
class Upgradable;
class Updatable;
class GameState
{

private:
    int simulationTime;

// TODO this should be private but it won't compile anymore
public:
    GameState(unsigned long initialGas, unsigned long initialSupply, unsigned long initialsMinerals){
        gas = initialGas * GAS_FACTOR;
        minerals = initialsMinerals * MIN_FACTOR;
        supply = initialSupply;
        simulationTime = 0;
    }
    
    vector<EntityType> entityTypes;
    vector<Upgradable*> upgradeables;
    vector<Updatable*> updatables;
    vector<Producer*> producers;
    vector<Entity*> entities;

    // minerals and gas is stored multiplied by MIN_FACTOR and GAS_FACTOR respectively
    const unsigned long MIN_FACTOR = 100, GAS_FACTOR = 100;
    unsigned long minerals, gas, supply;

    //This bitset contains a setted bit if this entity is already created by us
    //it is initialized by 50 to be sure to have enough capacity for our terran entities
    std::bitset<50> constructedBitset = 0;
    /*
     * Supply is a ressource that gives a maximum to how many units a player can control.
     * Some units and buildings increase the maximum supply this acts like a capacity.
     * Note that maximumSupply can never be increased over 200.
     * Most units use a certain amount of supply. This increases the usedSupply.
     * So maximumSupply - usedSupply gives the available amount of supply.
     */
    unsigned long usedSupply, maximumSupply;

    bool hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply);
    bool hasEnoughMinerals(unsigned long amount);
    bool hasEnoughVespine(unsigned long amount);
    bool hasEnoughSupply(unsigned long amount);

    //This method checks if type is existing/already created in our current state
    bool hasEntity(EntityType type);

    void consumeEnoughMinerals(unsigned long amount);
    void consumeEnoughVespine(unsigned long amount);
    void consumeEnoughSupply(unsigned long amount);

    void addMinerals(unsigned long amount);
    void addVespine(unsigned long amount);
    void increaseSupply(unsigned long amount);

    unsigned long getMinerals();

    void addEntity(EntityType type, unsigned long amount);
    void removeEntity(Entity& entity);
    void changeEntity(Entity& old, Entity& theNew);

    int getSimulationTime();
    void incrementSimulationTime();

	//contains all created entities
    vector<EntityType>& getEntities(EntityType& type);
	
	//following three vectors only contain pointers to specific elements of the above one
	//contains references to the getEntities vector (views)
    vector<Upgradable*>& getUpgradeables();
    vector<Updatable*>& getUpdatables();
    vector<Producer*>& getProducers();
};
