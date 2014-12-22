#include "BuildOrder.hpp"


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

    {TERRAN_PLANETARY_FORTRESS, {TERRAN_ENGINEERING_BAY, TERRAN_REFINERY}},
    {TERRAN_MISSILE_TURRET, {TERRAN_ENGINEERING_BAY}},
    {TERRAN_SENSOR_TOWER, {TERRAN_ENGINEERING_BAY}},
    {TERRAN_MARINE, {TERRAN_BARRACKS}},
    {TERRAN_BARRACKS_TECH_LAB, {TERRAN_BARRACKS, TERRAN_REFINERY}},
    {TERRAN_MARAUDER, {TERRAN_BARRACKS_TECH_LAB}},
    {TERRAN_REAPER, {TERRAN_BARRACKS_TECH_LAB}},
    {TERRAN_GHOST, {TERRAN_BARRACKS_TECH_LAB, TERRAN_GHOST_ACADEMY}},
    {TERRAN_BARRACKS_REACTOR, {TERRAN_BARRACKS, TERRAN_REFINERY}},
    {TERRAN_FACTORY, {TERRAN_BARRACKS, TERRAN_REFINERY}},
    {TERRAN_FACTORY_TECH_LAB, {TERRAN_FACTORY}},
    {TERRAN_FACTORY_REACTOR, {TERRAN_FACTORY}},
    {TERRAN_SIEGE_TANK, {TERRAN_FACTORY_TECH_LAB}},
    {TERRAN_THOR, {TERRAN_FACTORY_TECH_LAB, TERRAN_ARMORY}},
    {TERRAN_HELLION, {TERRAN_FACTORY}},
    {TERRAN_BUNKER, {TERRAN_BARRACKS}},
    {TERRAN_GHOST_ACADEMY, {TERRAN_BARRACKS}},
    {TERRAN_STARPORT, {TERRAN_FACTORY}},
    {TERRAN_MEDIVAC, {TERRAN_STARPORT}},
    {TERRAN_VIKING, {TERRAN_STARPORT}},
    {TERRAN_FUSION_CORE, {TERRAN_STARPORT}},
    {TERRAN_STARPORT_TECH_LAB, {TERRAN_STARPORT}},
    {TERRAN_STARPORT_REACTOR, {TERRAN_STARPORT}},
    {TERRAN_RAVEN, {TERRAN_STARPORT_TECH_LAB}},
    {TERRAN_BATTLECRUISER, {TERRAN_STARPORT_TECH_LAB, TERRAN_FUSION_CORE}},
    {TERRAN_BANSHEE, {TERRAN_STARPORT_TECH_LAB}}
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
}

BuildOrder::~BuildOrder()
{
    std::for_each(buildSteps.begin(), buildSteps.end(), [](BuildStep* bs) {delete bs;});
}

bool BuildOrder::doSanityCheck()
{
    if (buildSteps.size() == 0)
    {
        throw std::invalid_argument("There are no steps in the build list!");
    }

    int currentSupply = 0;

    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());
    set<EntityType> builtTypes;

    switch (this->race)
    {
        // Protoss and Zergonly has 10 supply at the start
        case EntityType::ZERG:
            currentSupply = 10;
            builtTypes.insert(ZERG_HATCHERY);
            builtTypes.insert(ZERG_OVERLORD);
            builtTypes.insert(ZERG_DRONE);
        break;
        case EntityType::PROTOSS:
            currentSupply = 10;
            builtTypes.insert(PROTOSS_NEXUS);
            builtTypes.insert(PROTOSS_PROBE);
            break;
        case EntityType::TERRAN:
            builtTypes.insert(TERRAN_COMMAND_CENTER);
            builtTypes.insert(TERRAN_SCV);
            currentSupply = 11;
            break;
        default:
            throw std::invalid_argument("Unknown race");
    }

    currentSupply -= 5; // Subtract supply for the workers

    for (BuildStep* step : buildSteps)
    {
        EntityType stepType = step->getEntityType();
        if (stepType == NONE)
        {
            throw std::invalid_argument(step->getName());
            throw std::invalid_argument("Did not recognize entityType");
        }

        if (this->race != Entity::typeToRace(stepType))
        {
            throw std::invalid_argument("Race mismatch: TODO MEANINGFUL ERROR MESSAGE");
        }

        currentSupply -= supply[stepType];
#ifdef DEBUG
        std::cerr << "Parsed: " 
                  << BuildStep::entityTypeToString[stepType] 
                  << " current supply: " 
                  << currentSupply 
                  << std::endl;;
#endif
        if (currentSupply < 0)
        {
            throw std::invalid_argument("Not enough supply");
        }
    }

    // TODO calculate max time and set this in BuildOrder maybe?
    // TODO calculate if supply is possible
    // TODO calculate if basic dependencies are possible


    for (BuildStep* step : buildSteps)
    {
        EntityType stepType = step->getEntityType(); 
        vector<EntityType> deps = BuildOrder::dependencies[stepType];

        for (EntityType d : deps)
        {
           if (!builtTypes.count(d))
           {
               ostringstream oss;
               oss << "Basic dependency not met: ";
               oss << BuildStep::entityTypeToString[stepType];
               oss << " depends on: ";
               oss << BuildStep::entityTypeToString[d];
               oss << endl;
              throw std::invalid_argument(oss.str());
           }
        } 

        builtTypes.insert(stepType);
    }

    return true;
}

BuildStep* BuildOrder::getNextStep()
{
    if(iterator != buildSteps.end())
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
        ++iterator;
    }
}

EntityType BuildOrder::getRace()
{
    return this->race;
}
