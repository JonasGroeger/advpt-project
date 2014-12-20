#include <entities/terran/Marine.hpp>
#include "GameState.hpp"
#include "Game.hpp"

bool GameState::hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply){
	return hasEnoughMinerals(minerals)
			&& hasEnoughVespine(vespine)
			&& hasEnoughSupply(supply);
}

bool GameState::hasEnoughMinerals(unsigned long amount) const
{
    return amount * FACTOR <= minerals;
}

bool GameState::hasEnoughVespine(unsigned long amount) const
{
    return amount * FACTOR <= gas;
}

bool GameState::hasEnoughSupply(unsigned long amount) const
{
    return amount <= (maximumSupply-usedSupply);
}

bool GameState::hasEntity(EntityType type) const
{
    return constructedBitset.test(type);
}

bool GameState::hasEntityInProduction(EntityType type) const
{
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
    usedSupply += amount;
}

void GameState::addMineralsWithFactor(unsigned long amount)
{
    minerals += amount;
}

void GameState::addVespineWithFactor(unsigned long amount)
{
    gas += amount;
}

void GameState::increaseSupply(unsigned long amount)
{
    maximumSupply = (maximumSupply + amount <= 200) ? (maximumSupply + amount) : (200);
}

unsigned long GameState::getMinerals() const
{
    return minerals / FACTOR;
}

unsigned long GameState::getVespine() const
{
    return gas / FACTOR;
}

unsigned long GameState::getUsedSupply() const
{
    return usedSupply;
}

unsigned long GameState::getAvailableSupply() const
{
    return maximumSupply - usedSupply;
}

void GameState::notifyEntityIsBeingProduced(EntityType type){
    entitiesInConstruction.set(type);
}

void GameState::setAvailableEntityType(EntityType type){
	constructedBitset.set(type);
}

void GameState::addEntity(EntityType type, unsigned long amount)
{
	Entity* new_unit;
    for (unsigned long i = 0; i < amount; i++)
    {
		switch(type) {
			case TERRAN_COMMAND_CENTER:
				new_unit = new CommandCenter();
                increaseSupply(11);
				break;
			case TERRAN_SCV:
				new_unit = new SCV(); 
				break;
			case TERRAN_BARRACKS:
				new_unit = new Barracks();
				break;
			case TERRAN_SUPPLY_DEPOT:
				new_unit = new SupplyDepot();
                increaseSupply(8);
				break;
			case TERRAN_MARINE:
				new_unit = new Marine();
				break;
			case TERRAN_REFINERY:
				new_unit = new Refinery();
                vespeneSlots += 3;
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
            if (hasOpenVespeneSlot())
            {
                worker->setTypeOfWork(TypeOfWork::Vespine);
            }
            else
            {
                worker->setTypeOfWork(TypeOfWork::Minerals);
            }
        }
		entities.push_back(new_unit);
		constructedBitset.set(type);

        if (logger != nullptr)
        {
            logger->printBuildEndMessage(type);
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

void GameState::setMaxSimTime(int time)
{
    maxTime = time;
}

bool GameState::maxSimTimeReached() const
{
    return (simulationTime >= maxTime);
}

int GameState::getSimulationTime() const
{
    return this->simulationTime;
}

void GameState::incrementSimulationTime() {
    this->simulationTime++;
}

bool GameState::hasOpenVespeneSlot()
{
    unsigned int usedSlots = 0;
    for (Worker* wrk : workers)
    {
        if (wrk->getTypeOfWork() == TypeOfWork::Vespine)
        {
            usedSlots ++;
        }
    }

    return vespeneSlots > usedSlots;
}

const vector<EntityType>& GameState::getEntities(EntityType& type) const
{
    return this->entityTypes;
}

const vector<Upgradable*>& GameState::getUpgradeables() const
{
    return upgradeables;
}

const vector<Updatable*>& GameState::getUpdatables() const
{
    return updatables;
}

const vector<Producer*>& GameState::getProducers() const
{
    return producers;
}

const vector<Worker*>& GameState::getWorkers() const
{
    return workers;
}

void GameState::registerLogger(Game *newLogger)
{
    logger = newLogger;
}
void GameState::unregisterLogger()
{
    logger = nullptr;
}
