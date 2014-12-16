#pragma once

#include <vector>

#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "Producer.hpp"
#include "Entity.hpp"

using std::vector;

#define GAS         ( gas * 100 )
#define MINERALS    ( minerals * 100 )

class Upgradable;
class Updatable;

class GameState
{

private:
    int simulationTime;

// TODO this should be private but it won't compile anymore
public:
    GameState(unsigned long initialGas, unsigned long initialSupply, unsigned long initialsMinerals){
        gas = initialGas;
        minerals = initialsMinerals;
        supply = initialSupply;
    }
    
    vector<EntityType> entityTypes;
    vector<Upgradable> upgradeables;
    vector<Updatable> updatables;
    vector<Producer> producers;

    // minerals and gas is stored multiplied by MIN_FACTOR and GAS_FACTOR respectively
    const unsigned long MIN_FACTOR = 100, GAS_FACTOR = 100;
    unsigned long minerals, gas, supply;

    /*
     * Supply is a ressource that gives a maximum to how many units a player can control.
     * Some units and buildings increase the maximum supply this acts like a capacity.
     * Note that maximumSupply can never be increased over 200.
     * Most units use a certain amount of supply. This increases the usedSupply.
     * So maximumSupply - usedSupply gives the available amount of supply.
     */
    unsigned long usedSupply, maximumSupply;

    bool hasEnoughMinerals(unsigned long amount);
    bool hasEnoughVespine(unsigned long amount);
    bool hasEnoughSupply(unsigned long amount);
    bool hasEntity(EntityType& type, unsigned long amount);

    void consumeEnoughMinerals(unsigned long amount);
    void consumeEnoughVespine(unsigned long amount);
    void consumeEnoughSupply(unsigned long amount);

    void addMinerals(unsigned long amount);
    void addVespine(unsigned long amount);
    void increaseSupply(unsigned long amount);

    void addEntity(EntityType& type, unsigned long amount);
    void removeEntity(Entity& entity);
    void changeEntity(Entity& old, Entity& theNew);

    int getSimulationTime();
    void incrementSimulationTime();

	//contains all created entities
    vector<EntityType>& getEntities(EntityType& type);
	
	//following three vectors only contain pointers to specific elements of the above one
	//contains references to the getEntities vector (views)
    vector<Upgradable>& getUpgradeables();
    vector<Updatable>& getUpdatables();
    vector<Producer>& getProducers();
};
