#include "BuildOrder.hpp"


map<EntityType, vector<EntityType>> BuildOrder::dependencies =
{
    {ZERG_OVERSEER, {ZERG_EXTRACTOR}}, // No Overlord neede because we start with one
    {ZERG_SPORE_CRAWLER, {ZERG_EVOLUTION_CHAMBER}},
    {ZERG_QUEEN, {ZERG_SPAWNING_POOL}},
    {ZERG_ZERGLING, {ZERG_SPAWNING_POOL}},
    {ZERG_LAIR, {ZERG_EXTRACTOR}},
    {ZERG_SPINE_CRAWLER, {ZERG_SPAWNING_POOL}},
    {ZERG_ROACH_WARREN, {ZERG_SPAWNING_POOL}},
    {ZERG_BANELING_NEST, {ZERG_SPAWNING_POOL, ZERG_EXTRACTOR}},
    {ZERG_HYDRALISK_DEN, {ZERG_LAIR, ZERG_EXTRACTOR}},
    {ZERG_INFESTATION_PIT, {ZERG_LAIR, ZERG_EXTRACTOR}},
    {ZERG_ROACH, {ZERG_ROACH_WARREN, ZERG_EXTRACTOR}},
    {ZERG_BANELING, {ZERG_BANELING_NEST, ZERG_EXTRACTOR}},
    {ZERG_HYDRALISK, {ZERG_HYDRALISK_DEN, ZERG_EXTRACTOR}},
    {ZERG_NYDUS_NETWORK, {ZERG_LAIR}}, // I am too lazy to write extractor it is transitiv anyway
    {ZERG_ULTRALIK_CAVERN, {ZERG_HIVE}},
    {ZERG_INFESTOR, {ZERG_INFESTATION_PIT}},
    {ZERG_SPIRE, {ZERG_LAIR}},
    {ZERG_NYDUS_WORM, {ZERG_NYDUS_NETWORK}},
    {ZERG_ULTRALISK, {ZERG_ULTRALIK_CAVERN}},
    {ZERG_GREATER_SPIRE, {ZERG_HIVE}},
    {ZERG_BROOD_LORD, {ZERG_GREATER_SPIRE, ZERG_CORRUPTOR}},
    {ZERG_CORRUPTOR, {ZERG_SPIRE}},
    {ZERG_MUTALISK, {ZERG_SPIRE}},

    {PROTOSS_ZEALOT, {PROTOSS_GATEWAY}},
    {PROTOSS_STALKER, {PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR}},
    {PROTOSS_SENTRY, {PROTOSS_GATEWAY, PROTOSS_ASSIMILATOR, PROTOSS_CYBERNETICS_CORE}},
    {PROTOSS_WARP_PRISM, {PROTOSS_ROBOTICS_FACILITY}},
    {PROTOSS_IMMORTAL, {PROTOSS_ROBOTICS_FACILITY}},
    {PROTOSS_OBSERVER, {PROTOSS_ROBOTICS_FACILITY}},
    {PROTOSS_COLOSSUS, {PROTOSS_ROBOTICS_BAY}},
    {PROTOSS_GATEWAY, {PROTOSS_PYLON}},
    {PROTOSS_CYBERNETICS_CORE, {PROTOSS_GATEWAY}},
    {PROTOSS_ROBOTICS_FACILITY, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
    {PROTOSS_ROBOTICS_BAY, {PROTOSS_ROBOTICS_FACILITY}},
    {PROTOSS_HIGH_TEMPLAR, {PROTOSS_TEMPLAR_ARCHIVES, PROTOSS_GATEWAY}},
    {PROTOSS_TEMPLAR_ARCHIVES, {PROTOSS_TWILIGHT_COUNCIL}},
    {PROTOSS_TWILIGHT_COUNCIL, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
    {PROTOSS_DARK_TEMPLAR, {PROTOSS_DARK_SHRINE, PROTOSS_GATEWAY}},
    {PROTOSS_DARK_SHRINE, {PROTOSS_TWILIGHT_COUNCIL}},
    {PROTOSS_STARGATE, {PROTOSS_CYBERNETICS_CORE, PROTOSS_ASSIMILATOR}},
    {PROTOSS_MOTHERSHIP, {PROTOSS_FLEET_BEACON}},
    {PROTOSS_FLEET_BEACON, {PROTOSS_STARGATE}},
    {PROTOSS_FORGE, {PROTOSS_PYLON}},
    {PROTOSS_PHOTON_CANNON, {PROTOSS_FORGE}},
    {PROTOSS_PHOENIX, {PROTOSS_STARGATE}},
    {PROTOSS_VOID_RAY, {PROTOSS_STARGATE}},
    {PROTOSS_CARRIER, {PROTOSS_FLEET_BEACON}},

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

    this->race = Entity::typeToRace(buildSteps[0]->getEntityType());
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
    }

    // TODO calculate max time and set this in BuildOrder maybe?
    // TODO calculate if supply is possible
    // TODO calculate if basic dependencies are possible

    set<EntityType> builtTypes;

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
