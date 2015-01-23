#include <entities/protoss/ProductionBuildings.hpp>
#include <entities/zerg/ProductionBuildings.hpp>
#include <entities/UpgradableUnit.hpp>
#include <entities/protoss/Probe.hpp>
#include "Game.hpp"

GameState::~GameState()
{
    std::for_each(entities.begin(), entities.end(), [](Entity *e) {
        delete e;
    });
}

bool GameState::hasEnoughEntities(EntityType type, int amount)
{
    int count = 0;
    for (auto entry : entities)
    {
        if (entry->getType() == type)
        {
            ++count;
        }
    }
    if (count >= amount)
    {
        return true;
    }
    return false;
}

bool GameState::hasEnough(unsigned long minerals, unsigned long vespine, unsigned long supply)
{
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
    return amount <= (maximumSupply - usedSupply);
}

bool GameState::hasEntity(EntityType type) const
{
    return constructedBitset.test(type);
}

bool GameState::hasEntityInProduction(EntityType type) const
{
    return entitiesInConstruction.test(type);
}

void GameState::consumeEnoughEntities(EntityType type, int amount)
{

    long num_entites = std::count_if(entities.begin(), entities.end(), [type](Entity *entry) {
        return entry->getType() == type;
    });

    //If we removed all occurences, we dont have that entity anymore
    if (num_entites <= amount)
    {
        constructedBitset.reset(type);
    }

    int deletedCount = 0;
    auto it = std::remove_if(entities.begin(), entities.end(), [amount, &deletedCount, type](Entity *entry) {
        if (deletedCount < amount && (entry->getType() == type))
        {
            ++deletedCount;
            return true;
        }
        return false;
    });
    entities.erase(it, entities.end());
}

void GameState::consumeDrone(Drone *drone)
{
    // We can be sure the drone is idle because the drone itself called this method
    for (auto it = updatables.begin(); it != updatables.end(); it++)
    {
        if (*it == drone)
        {
            updatables.erase(it);
            break;
        }
    }

    for (auto it = workers.begin(); it != workers.end(); it++)
    {
        if (*it == drone)
        {
            workers.erase(it);
            break;
        }
    }

    for (auto it = entities.begin(); it != entities.end(); it++)
    {
        if (*it == drone)
        {
            entities.erase(it);
            break;
        }
    }

    for (auto it = upgradeables.begin(); it != upgradeables.end(); it++)
    {
        if (*it == drone)
        {
            upgradeables.erase(it);
            break;
        }
    }

    delete drone;
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

void GameState::consumeEnough(unsigned long minerals, unsigned long vespine, unsigned long supply)
{
    consumeEnoughMinerals(minerals);
    consumeEnoughVespine(vespine);
    consumeEnoughSupply(supply);
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

void GameState::notifyEntityIsBeingProduced(EntityType type)
{
    entitiesInConstruction.set(type);
}

void GameState::setAvailableEntityType(EntityType type)
{
    constructedBitset.set(type);
}

void GameState::addEntity(EntityType type, unsigned long amount)
{
    Entity *new_unit;
    for (unsigned long i = 0; i < amount; i++)
    {
        switch (type)
        {
            ////////////////////////////////////////////
            // Complex(ish) terran entities. vvvvvvvv //
            ////////////////////////////////////////////
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
                new_unit = new SimpleEntity<TERRAN_SUPPLY_DEPOT>();
                increaseSupply(8);
                break;
            case TERRAN_REFINERY:
                new_unit = new SimpleEntity<TERRAN_REFINERY>();
                vespeneSlots += 3;
                break;
            case TERRAN_FACTORY:
                new_unit = new Factory();
                break;
            case TERRAN_STARPORT:
                new_unit = new Starport();
                break;
            case TERRAN_BARRACKS_REACTOR:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new BarracksReactor();
                break;
            case TERRAN_BARRACKS_TECH_LAB:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new BarracksTechLab();
                break;
            case TERRAN_FACTORY_TECH_LAB:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new FactoryTechLab();
                break;
            case TERRAN_FACTORY_REACTOR:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new FactoryReactor();
                break;
            case TERRAN_STARPORT_TECH_LAB:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new StarportTechLab();
                break;
            case TERRAN_STARPORT_REACTOR:
                // Ignore, we only get these entity out of an upgrade
                // new_unit = new StarportReactor();
                break;
            case TERRAN_PLANETARY_FORTRESS:
                new_unit = new PlanetaryFortress();
                break;

                ////////////////////////////////////////////
                // Simple terran entities. vvvvvvvvvvvvvv //
                ////////////////////////////////////////////
            case TERRAN_MARINE:
                new_unit = new SimpleEntity<TERRAN_MARINE>();
                break;
            case TERRAN_MARAUDER:
                new_unit = new SimpleEntity<TERRAN_MARAUDER>();
                break;
            case TERRAN_BATTLECRUISER:
                new_unit = new SimpleEntity<TERRAN_BATTLECRUISER>();
                break;
            case TERRAN_HELLION:
                new_unit = new SimpleEntity<TERRAN_HELLION>();
                break;
            case TERRAN_ARMORY:
                new_unit = new SimpleEntity<TERRAN_ARMORY>();
                break;
            case TERRAN_BANSHEE:
                new_unit = new SimpleEntity<TERRAN_BANSHEE>();
                break;
            case TERRAN_BUNKER:
                new_unit = new SimpleEntity<TERRAN_BUNKER>();
                break;
            case TERRAN_REAPER:
                new_unit = new SimpleEntity<TERRAN_REAPER>();
                break;
            case TERRAN_GHOST:
                new_unit = new SimpleEntity<TERRAN_GHOST>();
                break;
            case TERRAN_GHOST_ACADEMY:
                new_unit = new SimpleEntity<TERRAN_GHOST_ACADEMY>();
                break;
            case TERRAN_RAVEN:
                new_unit = new SimpleEntity<TERRAN_RAVEN>();
                break;
            case TERRAN_FUSION_CORE:
                new_unit = new SimpleEntity<TERRAN_FUSION_CORE>();
                break;
            case TERRAN_VIKING:
                new_unit = new SimpleEntity<TERRAN_VIKING>();
                break;
            case TERRAN_MEDIVAC:
                new_unit = new SimpleEntity<TERRAN_MEDIVAC>();
                break;
            case TERRAN_SIEGE_TANK:
                new_unit = new SimpleEntity<TERRAN_SIEGE_TANK>();
                break;
            case TERRAN_THOR:
                new_unit = new SimpleEntity<TERRAN_THOR>();
                break;
            case TERRAN_ORBITAL_COMMAND:
                new_unit = new SimpleEntity<TERRAN_ORBITAL_COMMAND>();
                break;
            case TERRAN_ENGINEERING_BAY:
                new_unit = new SimpleEntity<TERRAN_ENGINEERING_BAY>();
                break;
            case TERRAN_MISSILE_TURRET:
                new_unit = new SimpleEntity<TERRAN_MISSILE_TURRET>();
                break;
            case TERRAN_SENSOR_TOWER:
                new_unit = new SimpleEntity<TERRAN_SENSOR_TOWER>();
                break;

                ////////////////////////////////////////////
                // Complex(ish) protoss entities. vvvvvvv //
                ////////////////////////////////////////////
            case PROTOSS_ASSIMILATOR:
                new_unit = new SimpleEntity<PROTOSS_ASSIMILATOR>();
                vespeneSlots += 3;
                break;
            case PROTOSS_PYLON:
                new_unit = new SimpleEntity<PROTOSS_PYLON>();
                increaseSupply(8);
                break;
            case PROTOSS_NEXUS:
                new_unit = new Nexus();
                increaseSupply(10);
                break;
            case PROTOSS_PROBE:
                new_unit = new Probe();
                break;
            case PROTOSS_GATEWAY:
                new_unit = new Gateway();
                break;
            case PROTOSS_ROBOTICS_FACILITY:
                new_unit = new RoboticsFacility();
                break;
            case PROTOSS_STARGATE:
                new_unit = new Stargate();
                break;

                ////////////////////////////////////////////
                // Simple protoss entities. vvvvvvvvvvvvv //
                ////////////////////////////////////////////
            case PROTOSS_FORGE:
                new_unit = new SimpleEntity<PROTOSS_FORGE>();
                break;
            case PROTOSS_COLOSSUS:
                new_unit = new SimpleEntity<PROTOSS_COLOSSUS>();
                break;
            case PROTOSS_PHOTON_CANNON:
                new_unit = new SimpleEntity<PROTOSS_PHOTON_CANNON>();
                break;
            case PROTOSS_PHOENIX:
                new_unit = new SimpleEntity<PROTOSS_PHOENIX>();
                break;
            case PROTOSS_VOID_RAY:
                new_unit = new SimpleEntity<PROTOSS_VOID_RAY>();
                break;
            case PROTOSS_CARRIER:
                new_unit = new SimpleEntity<PROTOSS_CARRIER>();
                break;
            case PROTOSS_FLEET_BEACON:
                new_unit = new SimpleEntity<PROTOSS_FLEET_BEACON>();
                break;
            case PROTOSS_DARK_SHRINE:
                new_unit = new SimpleEntity<PROTOSS_DARK_SHRINE>();
                break;
            case PROTOSS_TEMPLAR_ARCHIVES:
                new_unit = new SimpleEntity<PROTOSS_TEMPLAR_ARCHIVES>();
                break;
            case PROTOSS_ROBOTICS_BAY:
                new_unit = new SimpleEntity<PROTOSS_ROBOTICS_BAY>();
                break;
            case PROTOSS_OBSERVER:
                new_unit = new SimpleEntity<PROTOSS_OBSERVER>();
                break;
            case PROTOSS_IMMORTAL:
                new_unit = new SimpleEntity<PROTOSS_IMMORTAL>();
                break;
            case PROTOSS_WARP_PRISM:
                new_unit = new SimpleEntity<PROTOSS_WARP_PRISM>();
                break;
            case PROTOSS_SENTRY:
                new_unit = new SimpleEntity<PROTOSS_SENTRY>();
                break;
            case PROTOSS_ZEALOT:
                new_unit = new SimpleEntity<PROTOSS_ZEALOT>();
                break;
            case PROTOSS_STALKER:
                new_unit = new SimpleEntity<PROTOSS_STALKER>();
                break;
            case PROTOSS_CYBERNETICS_CORE:
                new_unit = new SimpleEntity<PROTOSS_CYBERNETICS_CORE>();
                break;
            case PROTOSS_HIGH_TEMPLAR:
                new_unit = new SimpleEntity<PROTOSS_HIGH_TEMPLAR>();
                break;
            case PROTOSS_DARK_TEMPLAR:
                new_unit = new SimpleEntity<PROTOSS_DARK_TEMPLAR>();
                break;
            case PROTOSS_TWILIGHT_COUNCIL:
                new_unit = new SimpleEntity<PROTOSS_TWILIGHT_COUNCIL>();
                break;
            case PROTOSS_ARCHON:
                new_unit = new SimpleEntity<PROTOSS_ARCHON>();
                break;
            case PROTOSS_MOTHERSHIP:
                new_unit = new SimpleEntity<PROTOSS_MOTHERSHIP>();
                break;

                ////////////////////////////////////////////
                // Complex(ish) zerg entities. vvvvvvvvvv //
                ////////////////////////////////////////////
            case ZERG_LARVA_HELPER:
                larvaHelper = new LarvaHelper();
                new_unit = static_cast<Entity *> (larvaHelper);
                break;
            case ZERG_HATCHERY:
                new_unit = new Hatchery();
                increaseSupply(2);
                addHatchery();
                break;
            case ZERG_DRONE:
                new_unit = new Drone();
                break;
            case ZERG_OVERLORD:
                new_unit = new UpgradableUnit<ZERG_OVERLORD, ZERG_OVERSEER, 50, 50, 0, 17>();
                increaseSupply(8);
                break;
            case ZERG_EXTRACTOR:
                new_unit = new SimpleEntity<ZERG_EXTRACTOR>();
                vespeneSlots += 3;
                break;
            case ZERG_ZERGLING:
                new_unit = new UpgradableUnit<ZERG_ZERGLING, ZERG_BANELING, 25, 25, 1, 20>();
                break;
            case ZERG_BANELING:
                // Can only be obtained by upgrading a Baneling (pair).
                break;
            case ZERG_HIVE:
                // Can only be obtained by upgrading a Lair.
                break;
            case ZERG_SPIRE:
                new_unit = new UpgradableUnit<ZERG_SPIRE, ZERG_GREATER_SPIRE, 100, 150, 0, 100>();
                break;
            case ZERG_GREATER_SPIRE:
                // Can only be obtained by upgrading a Spire.
                break;
            case ZERG_CORRUPTOR:
                new_unit = new UpgradableUnit<ZERG_CORRUPTOR, ZERG_BROOD_LORD, 150, 150, 2, 34>();
                break;
            case ZERG_BROOD_LORD:
                // Can only be obtained by upgrading a Corruptor.
                break;
            case ZERG_NYDUS_NETWORK:
                new_unit = new NydusNetwork();
                break;

                ////////////////////////////////////////////
                // Simple zerg entities. vvvvvvvvvvvvvvvv //
                ////////////////////////////////////////////
            case ZERG_EVOLUTION_CHAMBER:
                new_unit = new SimpleEntity<ZERG_EVOLUTION_CHAMBER>();
                break;
            case ZERG_SPORE_CRAWLER:
                new_unit = new SimpleEntity<ZERG_SPORE_CRAWLER>();
                break;
            case ZERG_QUEEN:
                new_unit = new SimpleEntity<ZERG_QUEEN>();
                break;
            case ZERG_SPAWNING_POOL:
                new_unit = new SimpleEntity<ZERG_SPAWNING_POOL>();
                break;
            case ZERG_SPINE_CRAWLER:
                new_unit = new SimpleEntity<ZERG_SPINE_CRAWLER>();
                break;
            case ZERG_ROACH_WARREN:
                new_unit = new SimpleEntity<ZERG_ROACH_WARREN>();
                break;
            case ZERG_ROACH:
                new_unit = new SimpleEntity<ZERG_ROACH>();
                break;
            case ZERG_BANELING_NEST:
                new_unit = new SimpleEntity<ZERG_BANELING_NEST>();
                break;
            case ZERG_INFESTATION_PIT:
                new_unit = new SimpleEntity<ZERG_INFESTATION_PIT>();
                break;
            case ZERG_INFESTOR:
                new_unit = new SimpleEntity<ZERG_INFESTOR>();
                break;
            case ZERG_MUTALISK:
                new_unit = new SimpleEntity<ZERG_MUTALISK>();
                break;
            case ZERG_ULTRALIK_CAVERN:
                new_unit = new SimpleEntity<ZERG_ULTRALIK_CAVERN>();
                break;
            case ZERG_ULTRALISK:
                new_unit = new SimpleEntity<ZERG_ULTRALISK>();
                break;
            case ZERG_NYDUS_WORM:
                new_unit = new SimpleEntity<ZERG_NYDUS_WORM>();
                break;
            case ZERG_HYDRALISK_DEN:
                new_unit = new SimpleEntity<ZERG_HYDRALISK_DEN>();
                break;
            case ZERG_HYDRALISK:
                new_unit = new SimpleEntity<ZERG_HYDRALISK>();
                break;

                ////////////////////////////////////////////
                // The rest. vvvvvvvvvvvvvvvvvvvvvvvvvvvv //
                ////////////////////////////////////////////
            case NONE:
                return;
            default:
                return;
        }
        addEntityToVectors(new_unit);
    }
    // The message should only be printed once
    if (logger != nullptr)
    {
        return;
        printBuildEndMessage(type);
    }
}

void GameState::increaseLarva()
{
    if (larvaHelper != nullptr)
    {
        larvaHelper->increaseLarva();
    }
}

void GameState::addHatchery()
{
    if (larvaHelper != nullptr)
    {
        larvaHelper->addHatchery();
    }
}

int GameState::getMaxLarva() const
{
    if (larvaHelper != nullptr)
    {
        return larvaHelper->getMaxLarva();
    }
    return -1;
}

int GameState::getCurrentLarva() const
{
    if (larvaHelper != nullptr)
    {
        return larvaHelper->getCurrentLarva();
    }
    return -1;
}

void GameState::addCreatedEntity(Entity *entity)
{
    if (entity == nullptr)
    {
        return;
    }
    addEntityToVectors(entity);
}

void GameState::addEntityToVectors(Entity *entity)
{
    entities.push_back(entity);

    constructedBitset.set(entity->getType());

    if (entity->isUpgradable())
    {
        upgradeables.push_back(dynamic_cast<Upgradable *> (entity));
    }
    if (entity->isUpdatable())
    {
        updatables.push_back(dynamic_cast<Updatable *>(entity));
    }
    if (entity->isProducer())
    {
        producers.push_back(dynamic_cast<Producer *> (entity));
    }
    if (entity->isWorker())
    {
        Worker *worker = dynamic_cast<Worker *> (entity);
        workers.push_back(worker);
        worker->setTypeOfWork(TypeOfWork::Minerals);
    }
}


void GameState::removeEntity(Entity &entity)
{
    // TODO
}

void GameState::changeEntity(Entity &old, Entity &theNew)
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

unsigned int GameState::getSimulationTime() const
{
    return this->simulationTime;
}

void GameState::incrementSimulationTime()
{
    this->simulationTime++;
}

bool GameState::hasOpenVespeneSlot()
{
    unsigned int usedSlots = 0;
    for (Worker *wrk : workers)
    {
        if (wrk->getTypeOfWork() == TypeOfWork::Vespine)
        {
            usedSlots++;
        }
    }

    return vespeneSlots > usedSlots;
}

void GameState::reassignWorkers()
{
    unsigned long openSlots = vespeneSlots;
    bool first = true; // Always have at least one scv harvesting minerals
    for (Worker *wrk : workers)
    {
        switch (wrk->getTypeOfWork())
        {
            case TypeOfWork::Minerals:
            case TypeOfWork::Vespine:
            case TypeOfWork::Idle:
                if (openSlots > 0 && !first)
                {
                    wrk->setTypeOfWork(TypeOfWork::Vespine);
                    openSlots--;
                }
                else
                {
                    wrk->setTypeOfWork(TypeOfWork::Minerals);
                    first = false;
                }
                break;
            case TypeOfWork::Producing:
                break;
        }
    }
}

const vector<EntityType> &GameState::getEntities(EntityType &type) const
{
    return this->entityTypes;
}

const vector<Upgradable *> &GameState::getUpgradeables() const
{
    return upgradeables;
}

const vector<Updatable *> &GameState::getUpdatables() const
{
    return updatables;
}

const vector<Producer *> &GameState::getProducers() const
{
    return producers;
}

const vector<Worker *> &GameState::getWorkers() const
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

void GameState::printMessageProlog() const
{
    return;
    std::cout << std::left;
    std::cout << std::setw(5);
    std::cout << getSimulationTime();
    std::cout << std::setw(14);
}

void GameState::printBuildStartMessage(EntityType type) const
{
    return;
    printMessageProlog();
    std::cout << "build-start";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
}

void GameState::printBuildEndMessage(EntityType type) const
{
    return;
    printMessageProlog();
    std::cout << "build-end";
    std::cout << BuildStep::entityTypeToString[type];
    std::cout << std::endl;
#ifdef DEBUG
    printResourcesMessage();
#endif
    printWorkerMessage();
}

void GameState::printWorkerMessage() const
{
    return;
    int idleWorkers = 0;
    int vespineWorkers = 0;
    int mineralWorkers = 0;
    int producingWorkers = 0;
    auto workers = getWorkers();

    for (auto *worker : workers)
    {
        switch (worker->getTypeOfWork())
        {
            case TypeOfWork::Idle:
                idleWorkers++;
                break;
            case TypeOfWork::Vespine:
                vespineWorkers++;
                break;
            case TypeOfWork::Minerals:
                mineralWorkers++;
                break;
            case TypeOfWork::Producing:
                producingWorkers++;
                break;
        }
    }

    printMessageProlog();
    std::cout << "workers";
    std::cout << "minerals:" << mineralWorkers;
    std::cout << ",vespene:" << vespineWorkers;
    std::cout << std::endl;
}

void GameState::printResourcesMessage() const
{
    return;
    printMessageProlog();
    std::cout << "resources";
    std::cout << "minerals:" << getMinerals();
    std::cout << ",vespene:" << getVespine();
    std::cout << ",usedSupply:" << getUsedSupply();
    std::cout << ",availableSupply:" << getAvailableSupply();
#ifdef DEBUG
    std::cout << ",larva:" << getCurrentLarva() << '/' << getMaxLarva();
#endif
    std::cout << std::endl;
}
