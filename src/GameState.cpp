#include <ostream>
#include <iostream>
#include "GameState.hpp"

bool GameState::hasEnoughMinerals(unsigned long amount)
{
    return amount * FACTOR <= minerals;
}

bool GameState::hasEnoughVespine(unsigned long amount)
{
    return amount * FACTOR <= gas;
}

bool GameState::hasEnoughSupply(unsigned long amount)
{
    return amount * FACTOR <= (maximumSupply-usedSupply);
}

bool GameState::hasEntity(EntityType type)
{
    //TODO do we really need amount as a param here?
    return constructedBitset.test(type);
}

bool GameState::hasEntityInProduction(EntityType type){
    return entitiesInConstruction.test(type);
}

void GameState::consumeEnoughMinerals(unsigned long amount)
{
    minerals -= (amount * FACTOR);
}

void GameState::consumeEnoughVespine(unsigned long amount)
{
    gas -= (amount * FACTOR);
}

void GameState::consumeEnoughSupply(unsigned long amount)
{
    usedSupply += amount * FACTOR;
}

void GameState::addMineralsWithFactor(unsigned long amount)
{
    minerals += amount;
}

void GameState::addVespineWithFactor(unsigned long amount)
{
    gas += amount;
}

unsigned long GameState::getMinerals()
{
    return minerals / FACTOR;
}

void GameState::increaseSupply(unsigned long amount)
{
    maximumSupply = (maximumSupply + amount <= 200) ? (maximumSupply + amount) : (200);
}

void GameState::notifyEntityIsBeingProduced(EntityType type){
    entitiesInConstruction.set(type);
}

void GameState::addEntity(EntityType type, unsigned long amount)
{
	
	Entity* new_unit;
    for (unsigned long i = 0; i < amount; i++)
    {
		switch(type) {
			case TERRAN_COMMAND_CENTER:
				new_unit = new CommandCenter();
				break;
			case TERRAN_SCV:
				new_unit = new SCV(); 
				break;
			case TERRAN_BARRACKS:
				new_unit = new Barracks();
				break;
			case TERRAN_SUPPLY_DEPOT:
				new_unit = new SupplyDepot();
				break;
			case TERRAN_MARINE:
				new_unit = new Marine();
				break;
			case TERRAN_REFINERY:
				new_unit = new Refinery();
				break;
			case TERRAN_FACTORY:
				new_unit = new Factory();
				break;
			case TERRAN_STARPORT:
				new_unit = new Starport();
				break;
			case TERRAN_BATTLECRUISER:
				new_unit = new BattleCruiser();
				break;
			case TERRAN_HELLION:
				new_unit = new Hellion();
				break;
			case TERRAN_ARMORY:
				new_unit = new Armory();
				break;
			case TERRAN_BANSHEE:
				new_unit = new Banshee();
				break;
			case TERRAN_BARRACKS_REACTOR:
				new_unit = new BarracksReactor();
				break;
			case TERRAN_BARRACKS_TECH_LAB:
				new_unit = new BarracksTechLab();
				break;
			case TERRAN_BUNKER:
				new_unit = new Bunker();
				break;
			case TERRAN_REAPER:
				new_unit = new Reaper();
				break;
			case TERRAN_GHOST:
				new_unit = new Ghost();
				break;
			case TERRAN_GHOST_ACADEMY:
				new_unit = new GhostAcademy();
				break;
			case TERRAN_FACTORY_TECH_LAB:
				new_unit = new FactoryTechLab();
				break;
			case TERRAN_FACTORY_REACTOR:	
				new_unit = new FactoryReactor();
				break;
			case TERRAN_STARPORT_TECH_LAB:
				new_unit = new StarportTechLab();
				break;
			case TERRAN_STARPORT_REACTOR:
				new_unit = new StarportReactor();
				break;
			case TERRAN_RAVEN:
				new_unit = new Raven();
				break;
			case TERRAN_FUSION_CORE:
				new_unit = new FusionCore();
				break;
			case TERRAN_VIKING:
				new_unit = new Viking();
				break;
			case TERRAN_MEDIVAC:
				new_unit = new Medivac();
				break;
			case TERRAN_SIEGE_TANK:
				new_unit = new SiegeTank();				
				break;
			case TERRAN_THOR:
				new_unit = new Thor();
				break;
			case TERRAN_ORBITAL_COMMAND:
				new_unit = new OrbitalCommand();
				break;
			case TERRAN_ENGINEERING_BAY:
				new_unit = new EngineeringBay();
				break;
			case TERRAN_PLANETARY_FORTRESS:
				new_unit = new PlanetaryFortress();
				break;
			case TERRAN_MISSILE_TURRET:
				new_unit = new MissileTurret();
				break;
			case TERRAN_SENSOR_TOWER:
				new_unit = new SensorTower();
				break;
			case NONE:
				break;
			default:
				break;		
			
		}
		if(new_unit->isUpgradable()) 
		{ 
			upgradeables.push_back(dynamic_cast<Upgradable*> (new_unit)); 
		}
		if(new_unit->isUpdatable())
		{
			updatables.push_back(dynamic_cast<Updatable*>(new_unit));
		}
		if(new_unit->isProducer()) 
		{
			producers.push_back(dynamic_cast<Producer*> (new_unit));
		}
        if (new_unit->isWorker())
        {
            Worker *worker = dynamic_cast<Worker*> (new_unit);
			workers.push_back(worker);
            worker->setTypeOfWork(TypeOfWork::Minerals);
        }
		entities.push_back((new_unit));
		constructedBitset.set(type);
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

bool GameState::maxSimTimeReached(){
    return (simulationTime >= maxTime);
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

vector<Worker*>& GameState::getWorkers() {
    return workers;
}

bool GameState::hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply)
{
    return hasEnoughMinerals(minerals) && hasEnoughVespine(vespine) && hasEnoughSupply(supply);
}
