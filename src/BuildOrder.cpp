#include "BuildOrder.hpp"
#include "Debug.hpp"


map<EntityType, vector<EntityType>> BuildOrder::dependencies =
        {
                {ZERG_OVERSEER, {ZERG_LAIR, ZERG_EXTRACTOR}}, // No Overlord neede because we start with one
                {ZERG_SPORE_CRAWLER, {ZERG_EVOLUTION_CHAMBER}},
                {ZERG_QUEEN, {ZERG_SPAWNING_POOL}},
                {ZERG_ZERGLING, {ZERG_SPAWNING_POOL}},
                {ZERG_LAIR, {ZERG_SPAWNING_POOL, ZERG_HATCHERY, ZERG_EXTRACTOR}},
                {ZERG_SPAWNING_POOL, {ZERG_HATCHERY}},
                {ZERG_SPINE_CRAWLER, {ZERG_SPAWNING_POOL}},
                {ZERG_ROACH_WARREN, {ZERG_SPAWNING_POOL}},
                {ZERG_BANELING_NEST, {ZERG_SPAWNING_POOL, ZERG_EXTRACTOR}},
                {ZERG_HYDRALISK_DEN, {ZERG_LAIR, ZERG_EXTRACTOR}},
                {ZERG_INFESTATION_PIT, {ZERG_LAIR, ZERG_EXTRACTOR}},
                {ZERG_EVOLUTION_CHAMBER, {ZERG_HATCHERY}},
                {ZERG_ROACH, {ZERG_ROACH_WARREN, ZERG_EXTRACTOR}},
                {ZERG_BANELING, {ZERG_BANELING_NEST, ZERG_ZERGLING, ZERG_EXTRACTOR}},
                {ZERG_HYDRALISK, {ZERG_HYDRALISK_DEN}},
                {ZERG_NYDUS_NETWORK, {ZERG_LAIR, ZERG_EXTRACTOR}},
                {ZERG_ULTRALIK_CAVERN, {ZERG_HIVE, ZERG_EXTRACTOR}},
                {ZERG_HIVE, {ZERG_LAIR, ZERG_INFESTATION_PIT, ZERG_EXTRACTOR}},
                {ZERG_INFESTOR, {ZERG_INFESTATION_PIT, ZERG_EXTRACTOR}},
                {ZERG_SPIRE, {ZERG_LAIR, ZERG_EXTRACTOR}},
                {ZERG_NYDUS_WORM, {ZERG_NYDUS_NETWORK, ZERG_EXTRACTOR}},
                {ZERG_ULTRALISK, {ZERG_ULTRALIK_CAVERN, ZERG_EXTRACTOR}},
                {ZERG_GREATER_SPIRE, {ZERG_HIVE, ZERG_SPIRE, ZERG_EXTRACTOR}},
                {ZERG_BROOD_LORD, {ZERG_GREATER_SPIRE, ZERG_CORRUPTOR, ZERG_EXTRACTOR}},
                {ZERG_CORRUPTOR, {ZERG_SPIRE, ZERG_EXTRACTOR}},
                {ZERG_MUTALISK, {ZERG_SPIRE, ZERG_EXTRACTOR}},

                {PROTOSS_ZEALOT, {PROTOSS_GATEWAY}},
                {PROTOSS_STALKER, {PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR, PROTOSS_CYBERNETICS_CORE}},
                {PROTOSS_SENTRY, {PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR, PROTOSS_CYBERNETICS_CORE}},
                {PROTOSS_WARP_PRISM, {PROTOSS_ROBOTICS_FACILITY}},
                {PROTOSS_IMMORTAL, {PROTOSS_ROBOTICS_FACILITY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_OBSERVER, {PROTOSS_ROBOTICS_FACILITY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_COLOSSUS, {PROTOSS_ROBOTICS_BAY, PROTOSS_ROBOTICS_FACILITY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_GATEWAY, {PROTOSS_NEXUS, PROTOSS_PYLON}},
                {PROTOSS_CYBERNETICS_CORE, {PROTOSS_GATEWAY}},
                {PROTOSS_ROBOTICS_FACILITY, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_ROBOTICS_BAY, {PROTOSS_ROBOTICS_FACILITY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_HIGH_TEMPLAR, {PROTOSS_TEMPLAR_ARCHIVES, PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_TEMPLAR_ARCHIVES, {PROTOSS_TWILIGHT_COUNCIL, PROTOSS_ASSIMILATOR}},
                {PROTOSS_TWILIGHT_COUNCIL, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_DARK_TEMPLAR, {PROTOSS_DARK_SHRINE, PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR}},
                {PROTOSS_DARK_SHRINE, {PROTOSS_TWILIGHT_COUNCIL, PROTOSS_ASSIMILATOR}},
                {PROTOSS_STARGATE, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_MOTHERSHIP, {PROTOSS_NEXUS, PROTOSS_ASSIMILATOR, PROTOSS_FLEET_BEACON}},
                {PROTOSS_FLEET_BEACON, {PROTOSS_STARGATE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_FORGE, {PROTOSS_PYLON, PROTOSS_NEXUS}},
                {PROTOSS_PHOTON_CANNON, {PROTOSS_FORGE}},
                {PROTOSS_PHOENIX, {PROTOSS_STARGATE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_VOID_RAY, {PROTOSS_STARGATE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_CARRIER, {PROTOSS_FLEET_BEACON, PROTOSS_STARGATE, PROTOSS_ASSIMILATOR}},
                {PROTOSS_PROBE, {PROTOSS_NEXUS}},

                {TERRAN_PLANETARY_FORTRESS, {TERRAN_ENGINEERING_BAY, TERRAN_COMMAND_CENTER, TERRAN_REFINERY}},
                {TERRAN_MISSILE_TURRET, {TERRAN_ENGINEERING_BAY}},
                {TERRAN_SENSOR_TOWER, {TERRAN_ENGINEERING_BAY, TERRAN_REFINERY}},
                {TERRAN_MARINE, {TERRAN_BARRACKS}},
                {TERRAN_BARRACKS_TECH_LAB, {TERRAN_BARRACKS, TERRAN_REFINERY}},
                {TERRAN_BARRACKS, {TERRAN_SUPPLY_DEPOT}},
                {TERRAN_ORBITAL_COMMAND, {TERRAN_COMMAND_CENTER, TERRAN_BARRACKS}},
                {TERRAN_ENGINEERING_BAY, {TERRAN_COMMAND_CENTER}},

                {TERRAN_MARAUDER, {TERRAN_BARRACKS_TECH_LAB, TERRAN_REFINERY}},
                {TERRAN_REAPER, {TERRAN_BARRACKS_TECH_LAB, TERRAN_REFINERY}},
                {TERRAN_GHOST, {TERRAN_BARRACKS_TECH_LAB, TERRAN_GHOST_ACADEMY, TERRAN_REFINERY}},
                {TERRAN_BARRACKS_REACTOR, {TERRAN_BARRACKS, TERRAN_REFINERY}},
                {TERRAN_FACTORY, {TERRAN_BARRACKS, TERRAN_REFINERY}},

                {TERRAN_FACTORY_TECH_LAB, {TERRAN_FACTORY, TERRAN_REFINERY}},
                {TERRAN_FACTORY_REACTOR, {TERRAN_FACTORY, TERRAN_REFINERY}},
                {TERRAN_SIEGE_TANK, {TERRAN_FACTORY_TECH_LAB, TERRAN_REFINERY}},
                {TERRAN_THOR, {TERRAN_FACTORY_TECH_LAB, TERRAN_ARMORY, TERRAN_REFINERY}},
                {TERRAN_HELLION, {TERRAN_FACTORY}},
                {TERRAN_ARMORY, {TERRAN_FACTORY, TERRAN_REFINERY}},

                {TERRAN_BUNKER, {TERRAN_BARRACKS}},
                {TERRAN_GHOST_ACADEMY, {TERRAN_BARRACKS, TERRAN_REFINERY}},
                {TERRAN_STARPORT, {TERRAN_FACTORY, TERRAN_REFINERY}},
                {TERRAN_MEDIVAC, {TERRAN_STARPORT, TERRAN_REFINERY}},
                {TERRAN_VIKING, {TERRAN_STARPORT, TERRAN_REFINERY}},

                {TERRAN_FUSION_CORE, {TERRAN_STARPORT, TERRAN_REFINERY}},
                {TERRAN_STARPORT_TECH_LAB, {TERRAN_STARPORT, TERRAN_REFINERY}},
                {TERRAN_STARPORT_REACTOR, {TERRAN_STARPORT, TERRAN_REFINERY}},
                {TERRAN_RAVEN, {TERRAN_STARPORT_TECH_LAB, TERRAN_REFINERY}},
                {TERRAN_BATTLECRUISER, {TERRAN_STARPORT_TECH_LAB, TERRAN_FUSION_CORE, TERRAN_REFINERY}},
                {TERRAN_BANSHEE, {TERRAN_STARPORT_TECH_LAB, TERRAN_REFINERY}}

                // SCV ist nicht drin.
        };


map<EntityType, int> BuildOrder::supply =
        {
                // Buildings created from a Drone give +1 supply
                {ZERG_DRONE, 1},
                {ZERG_OVERLORD, -8},
                {ZERG_SPAWNING_POOL, -1},
                {ZERG_HATCHERY, 2},
                {ZERG_EXTRACTOR, -1},
                {ZERG_OVERSEER, 0},
                {ZERG_SPORE_CRAWLER, -1},
                {ZERG_QUEEN, 2},
                {ZERG_ZERGLING, 1},
                {ZERG_LAIR, 0},
                {ZERG_SPINE_CRAWLER, -1},
                {ZERG_ROACH_WARREN, -1},
                {ZERG_BANELING_NEST, -1},
                {ZERG_HYDRALISK_DEN, -1},
                {ZERG_INFESTATION_PIT, -1},
                {ZERG_ROACH, 2},
                {ZERG_BANELING, 1},
                {ZERG_HYDRALISK, 2},
                {ZERG_NYDUS_NETWORK, -1},
                {ZERG_ULTRALIK_CAVERN, -1},
                {ZERG_INFESTOR, 2},
                {ZERG_SPIRE, -1},
                {ZERG_NYDUS_WORM, 0},
                {ZERG_ULTRALISK, 6},
                {ZERG_GREATER_SPIRE, 0},
                {ZERG_BROOD_LORD, 2},
                {ZERG_CORRUPTOR, 2},
                {ZERG_MUTALISK, 2},

                {PROTOSS_PYLON, -8},
                {PROTOSS_NEXUS, -10},
                {PROTOSS_PROBE, 1},
                {PROTOSS_ZEALOT, 2},
                {PROTOSS_STALKER, 2},
                {PROTOSS_SENTRY, 2},
                {PROTOSS_WARP_PRISM, 2},
                {PROTOSS_IMMORTAL, 4},
                {PROTOSS_OBSERVER, 1},
                {PROTOSS_COLOSSUS, 6},
                {PROTOSS_GATEWAY, 0},
                {PROTOSS_CYBERNETICS_CORE, 0},
                {PROTOSS_ROBOTICS_FACILITY, 0},
                {PROTOSS_ROBOTICS_BAY, 0},
                {PROTOSS_HIGH_TEMPLAR, 2},
                {PROTOSS_TEMPLAR_ARCHIVES, 0},
                {PROTOSS_TWILIGHT_COUNCIL, 0},
                {PROTOSS_DARK_TEMPLAR, 2},
                {PROTOSS_DARK_SHRINE, 0},
                {PROTOSS_STARGATE, 0},
                {PROTOSS_MOTHERSHIP, 8},
                {PROTOSS_FLEET_BEACON, 0},
                {PROTOSS_FORGE, 0},
                {PROTOSS_PHOTON_CANNON, 0},
                {PROTOSS_PHOENIX, 2},
                {PROTOSS_VOID_RAY, 3},
                {PROTOSS_CARRIER, 6},

                {TERRAN_SCV, 1},
                {TERRAN_COMMAND_CENTER, -11},
                {TERRAN_SUPPLY_DEPOT, -8},
                {TERRAN_PLANETARY_FORTRESS, 0},
                {TERRAN_MISSILE_TURRET, 0},
                {TERRAN_SENSOR_TOWER, 0},
                {TERRAN_MARINE, 1},
                {TERRAN_BARRACKS_TECH_LAB, 0},
                {TERRAN_MARAUDER, 2},
                {TERRAN_REAPER, 1},
                {TERRAN_GHOST, 2},
                {TERRAN_BARRACKS_REACTOR, 0},
                {TERRAN_FACTORY, 0},
                {TERRAN_FACTORY_TECH_LAB, 0},
                {TERRAN_FACTORY_REACTOR, 0},
                {TERRAN_SIEGE_TANK, 3},
                {TERRAN_THOR, 6},
                {TERRAN_HELLION, 2},
                {TERRAN_BUNKER, 0},
                {TERRAN_GHOST_ACADEMY, 0},
                {TERRAN_STARPORT, 0},
                {TERRAN_MEDIVAC, 2},
                {TERRAN_VIKING, 2},
                {TERRAN_FUSION_CORE, 0},
                {TERRAN_STARPORT_TECH_LAB, 0},
                {TERRAN_STARPORT_REACTOR, 0},
                {TERRAN_RAVEN, 2},
                {TERRAN_BATTLECRUISER, 6},
                {TERRAN_BANSHEE, 3}
        };

BuildOrder::BuildOrder(BuildOrder& other)
{
    this->buildSteps = other.buildSteps;
    this->iterator = buildSteps.begin();
    this->race = other.race;
}

BuildOrder::BuildOrder(std::vector<BuildStep *> buildList)
{
    buildSteps = buildList;
    iterator = buildSteps.begin();

    doSanityCheck();
}

BuildOrder::BuildOrder(const char *file)
{
    ifstream infile(file);
    string line;
    while (getline(infile, line))
    {
        buildSteps.push_back(new BuildStep(line));
    }

    iterator = buildSteps.begin();

    doSanityCheck();

    cerr << "Created BuildOrder: " << buildSteps.end()-iterator<< endl;
}

BuildOrder::~BuildOrder()
{
    fprintf(stderr, "BuildOrder Destructor\n");
    std::for_each(buildSteps.begin(), buildSteps.end(), [](BuildStep *bs) {
        delete bs;
    });
}

void BuildOrder::clearBuildSteps() {
    buildSteps.clear();
}

/*
 * This method checks if there is enough supply provied to complete the list
 * If the list is empty this method returns true.
 * This method will not throw exceptions
 *
 * The list is assumed to pass checkIntegrity, but this will only be confirmed in debug mode
 *
 * For an introduction to this game mechanic see: http://wiki.teamliquid.net/starcraft2/Resources#Supply
 */
bool BuildOrder::isSupplyPossible()
{ 
#if DEBUG
    checkIntegrity();
#endif 

    if (buildSteps.size() == 0)
    {
        return true;
    }

    int availableSupply = 0; 
    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());

    /*
     * The different races start with different amount of supply.
     * Here the supply for 5 starting workers is already subtracted
     */
    switch (this->race)
    {
        // Protoss and Zergonly has 10 supply at the start
        case EntityType::ZERG:
            availableSupply = 5;
            break;
        case EntityType::PROTOSS:
            availableSupply = 5;
            break;
        case EntityType::TERRAN:
            availableSupply = 6;
            break;
        default:
            return false;
    }

    /*
     * We just subtract the supply of every stepType
     * If we have less than 0 supply available it is impossible to complete this list
     */
    for (BuildStep *step : buildSteps)
    {
        EntityType stepType = step->getEntityType();
        availableSupply -= supply[stepType];

        if (availableSupply < 0)
        {
            return false;
        }
    }

    return true;
}

/*
 * Checks basic requirements of the list and will throw execeptions if it fails.
 * This should not happen if BuildOrder ist used appropriately and automatic Integrity checks
 * are only enabled in DEBUG mode
 *
 * Empty lists are allowed
 *
 * Exceptions are thrown iff:
 *  - A build step has invalid race (should never happen)
 *  - A build step has EntityType NONE
 *  - Build steps with differen races appear
 */
void BuildOrder::checkIntegrity()
{
    if (buildSteps.size() == 0)
    {
        return;
    }

    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());
    switch (this->race)
    {
        case EntityType::ZERG:
        case EntityType::PROTOSS:
        case EntityType::TERRAN:
            break;
        default:
            // If this exception is thrown the first buildstep is broken
            throw std::invalid_argument("Unknown race");
    }
    for (BuildStep *step : buildSteps)
    {
        EntityType stepType = step->getEntityType();
        if (stepType == NONE)
        {
            std::string msg = "Did not recognize EntityType '" + step->getName() + "'.";
            throw std::invalid_argument(msg);
        }

        auto newEntityRace = Entity::typeToRace(stepType);
        if (this->race != newEntityRace)
        {
            std::string msg = "You cannot build unit '" + step->getName() + "' with your race.";
            throw std::invalid_argument(msg);
        }
    }
}

/*
 * This method checks if there are more than two vespene facilities in the list
 * If the list is empty this method returns true.
 * This method will not throw exceptions.
 *
 * The list is assumed to pass checkIntegrity, but this will only be confirmed in debug mode
 */
bool BuildOrder::isVespenePossible()
{
#ifdef DEBUG
    checkIntegrity();
#endif
    // Check if there are no more than 2 harvesting facilities in the buildlist
    // Issue: #43
    for (BuildStep *step: buildSteps)
    {
        unsigned int nHarvestingFacilities = 0;
        auto stepType = step->getEntityType();
        if (stepType == PROTOSS_ASSIMILATOR || stepType == TERRAN_REFINERY || stepType == ZERG_EXTRACTOR)
        {
            ++nHarvestingFacilities;
        }

        if (nHarvestingFacilities > 2)
        {
            return false;
        }
    }

    return true;
}

/*
 * This method checks if the dependencies of every step are met.
 * If the list is empty this method returns true.
 * This method will not throw exceptions.
 *
 * The list is assumed to pass checkIntegrity, but this will only be confirmed in debug mode
 */
bool BuildOrder::isDependenciesMet()
{
#ifdef DEBUG
    checkIntegrity();
#endif

    if (buildSteps.size() == 0)
    {
        return true;
    }

    // We but every EntityType we build into this set
    set<EntityType> builtTypes;
    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());

    switch (this->race)
    {
        // Protoss and Zergonly has 10 supply at the start
        case EntityType::ZERG:
            builtTypes.insert(ZERG_HATCHERY);
            builtTypes.insert(ZERG_OVERLORD);
            builtTypes.insert(ZERG_DRONE);
            break;
        case EntityType::PROTOSS:
            builtTypes.insert(PROTOSS_NEXUS);
            builtTypes.insert(PROTOSS_PROBE);
            break;
        case EntityType::TERRAN:
            builtTypes.insert(TERRAN_COMMAND_CENTER);
            builtTypes.insert(TERRAN_SCV);
            break;
        default:
            return false;
    }

    for (BuildStep *step : buildSteps)
    {
        EntityType stepType = step->getEntityType();
        vector<EntityType> deps = BuildOrder::dependencies[stepType];

        for (EntityType d : deps)
        {
            if (!builtTypes.count(d))
            {
                return false;
            }
        }

        builtTypes.insert(stepType);
    }

    return true;
}

/*
 * This method check if the buildList is possible to complete.
 * If the list is empty this method returns true
 * This method will not throw exceptions.
 *
 * The list is assumed to pass checkIntegrity, but this will only be confirmed in debug mode
 */
bool BuildOrder::isPossible()
{
#ifdef DEBUG
    checkIntegrity();
#endif
    return isSupplyPossible()
        && isVespenePossible()
        && isDependenciesMet();
}

BuildStep *BuildOrder::getNextStep()
{
    if (iterator != buildSteps.end())
    {
        return *iterator;
    }
    return nullptr;
}

bool BuildOrder::isDone()
{
    return iterator == buildSteps.end();
}

void BuildOrder::advance()
{
    if (iterator != buildSteps.end())
    {
        // cerr << "Advanced BuildOrder: " << buildSteps.end()-iterator<< endl;
        ++iterator;
    }
}

EntityType BuildOrder::getRace()
{
    return this->race;
}


void BuildOrder::addStepToBuildList(BuildStep* step) {
    buildSteps.push_back(step);
}

void BuildOrder::removeLastStepFromBuildList() {
    buildSteps.pop_back();
}

vector<BuildStep*> BuildOrder::getBuildList() {
    return buildSteps;
}
