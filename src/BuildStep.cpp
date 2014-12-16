#include "BuildStep.hpp"

map<string, EntityType> BuildStep::stringToEntityType = 
{
    {"assimilator", PROTOSS_ASSIMILATOR},
    {"forge", PROTOSS_FORGE},
    {"pylon", PROTOSS_PYLON},
    {"nexus", PROTOSS_NEXUS},
    {"photon_cannon", PROTOSS_PHOTON_CANNON},
    {"phoenix", PROTOSS_PHOENIX},
    {"void_ray", PROTOSS_VOID_RAY},
    {"carrier", PROTOSS_CARRIER},
    {"fleet_bacon", PROTOSS_FLEET_BEACON},
    {"dark_shrine", PROTOSS_DARK_SHRINE},
    {"templar_archives", PROTOSS_TEMPLAR_ARCHIVES},
    {"robotics_bay", PROTOSS_ROBOTICS_BAY},
    {"colossus", PROTOSS_COLOSSUS},
    {"observer", PROTOSS_OBSERVER},
    {"immortal", PROTOSS_IMMORTAL},
    {"warp_prism", PROTOSS_WARP_PRISM},
    {"sentry", PROTOSS_SENTRY},
    {"stalker", PROTOSS_STALKER},
    {"zealot", PROTOSS_ZEALOT},
    {"probe", PROTOSS_PROBE},
    {"gateway", PROTOSS_GATEWAY},
    {"cybernetics_core", PROTOSS_CYBERNETICS_CORE},
    {"robotics_facility", PROTOSS_ROBOTICS_FACILITY},
    {"high_templar", PROTOSS_HIGH_TEMPLAR},
    {"dark_templar", PROTOSS_DARK_TEMPLAR},
    {"twilight_council", PROTOSS_TWILIGHT_COUNCIL},
    {"stargate", PROTOSS_STARGATE},
    {"archon", PROTOSS_ARCHON},
    {"mothership", PROTOSS_MOTHERSHIP},
    //zerg
    {"hatchery", ZERG_HATCHERY},
    {"larva", ZERG_LARVA},
    {"evolution_chamber", ZERG_EVOLUTION_CHAMBER},
    {"crawler", ZERG_SPORE_CRAWLER},
    {"drone",ZERG_DRONE},
    {"queen",ZERG_QUEEN},
    {"lair",ZERG_LAIR},
    {"overseer",ZERG_OVERSEER},
    {"overlord",ZERG_OVERLORD},
    {"spwaning_pool",ZERG_SPAWNING_POOL},
    {"spine_crawler",ZERG_SPINE_CRAWLER},
    {"roach_warren",ZERG_ROACH_WARREN},
    {"baneling_nest",ZERG_BANELING_NEST},
    {"extractor",ZERG_EXTRACTOR},
    {"zergling",ZERG_ZERGLING},
    {"roach",ZERG_ROACH},
    {"baneling",ZERG_BANELING},
    {"infestation_pit",ZERG_INFESTATION_PIT},
    {"infestor",ZERG_INFESTOR},
    {"hive",ZERG_HIVE},
    {"spire",ZERG_SPIRE},
    {"greater_spire",ZERG_GREATER_SPIRE},
    {"corruptor",ZERG_CORRUPTOR},
    {"mutalisk",ZERG_MUTALISK},
    {"brood_lord",ZERG_BROOD_LORD},
    {"ultralisk_cavern",ZERG_ULTRALIK_CAVERN},
    {"ultralisk",ZERG_ULTRALISK},
    {"nydus_worm",ZERG_NYDUS_WORM},
    {"nydus_network",ZERG_NYDUS_NETWORK},
    {"hydralisk_den",ZERG_HYDRALISK_DEN},
    {"hydralisk",ZERG_HYDRALISK},
    //terran
    {"refinery",TERRAN_REFINERY},
    {"supply_depot",TERRAN_SUPPLY_DEPOT},
    {"barracks",TERRAN_BARRACKS},
    {"barrack_tech_lab",TERRAN_BARRACKS_TECH_LAB},
    {"barracks_reactor",TERRAN_BARRACKS_REACTOR},
    {"marauder",TERRAN_MARAUDER},
    {"reaper",TERRAN_REAPER},
    {"ghost",TERRAN_GHOST},
    {"ghost_academy",TERRAN_GHOST_ACADEMY},
    {"bunker",TERRAN_BUNKER},
    {"hellion",TERRAN_HELLION},
    {"marine",TERRAN_MARINE},
    {"factory",TERRAN_FACTORY},
    {"factory_tech_lab",TERRAN_FACTORY_TECH_LAB},
    {"factory_reactor",TERRAN_FACTORY_REACTOR},
    {"starport",TERRAN_STARPORT},
    {"starport_tech_lab",TERRAN_STARPORT_TECH_LAB},
    {"starport_reactor",TERRAN_STARPORT_REACTOR},
    {"raven",TERRAN_RAVEN},
    {"banshee",TERRAN_BANSHEE},
    {"battlecruiser",TERRAN_BATTLECRUISER},
    {"fusion_core",TERRAN_FUSION_CORE},
    {"viking",TERRAN_VIKING},
    {"medivac",TERRAN_MEDIVAC},
    {"siege_tank",TERRAN_SIEGE_TANK},
    {"thor",TERRAN_THOR},
    {"armory",TERRAN_ARMORY},
    {"orbital_command",TERRAN_ORBITAL_COMMAND},
    {"command_center",TERRAN_COMMAND_CENTER},
    {"svc",TERRAN_SVC},
    {"engineering_bay",TERRAN_ENGINEERING_BAY},
    {"planetary_fortress",TERRAN_PLANETARY_FORTRESS},
    {"missile_turret",TERRAN_MISSILE_TURRET},
    {"sensort_tower",TERRAN_SENSOR_TOWER},
};


map<EntityType, string> BuildStep::entityTypeToString;

map<string, BuildStepType> BuildStep::stringToBuildStepType =
{
    {"assimilator", BuildStepType::CONSTRUCT},
    {"forge", BuildStepType::CONSTRUCT},
    {"pylon", BuildStepType::CONSTRUCT},
    {"nexus", BuildStepType::CONSTRUCT},
    {"photon_cannon", BuildStepType::CONSTRUCT},
    {"phoenix", BuildStepType::PRODUCE},
    {"void_ray", BuildStepType::PRODUCE},
    {"carrier", BuildStepType::PRODUCE},
    {"fleet_bacon", BuildStepType::CONSTRUCT},
    {"dark_shrine", BuildStepType::CONSTRUCT},
    {"templar_archives", BuildStepType::CONSTRUCT},
    {"robotics_bay", BuildStepType::CONSTRUCT},
    {"colossus", BuildStepType::PRODUCE},
    {"observer", BuildStepType::PRODUCE},
    {"immortal", BuildStepType::PRODUCE},
    {"warp_prism", BuildStepType::PRODUCE},
    {"sentry", BuildStepType::PRODUCE},
    {"stalker", BuildStepType::PRODUCE},
    {"zealot", BuildStepType::PRODUCE},
    {"probe", BuildStepType::PRODUCE},
    {"gateway", BuildStepType::CONSTRUCT},
    {"cybernetics_core", BuildStepType::CONSTRUCT},
    {"robotics_facility", BuildStepType::CONSTRUCT},
    {"high_templar", BuildStepType::PRODUCE},
    {"dark_templar", BuildStepType::PRODUCE},
    {"twilight_council", BuildStepType::CONSTRUCT},
    {"stargate", BuildStepType::CONSTRUCT},
    {"archon", BuildStepType::PRODUCE},
    {"mothership", BuildStepType::PRODUCE}
};
map<BuildStepType, string> BuildStep::buildStepTypeToString;

BuildStep::BuildStep(string value)
{
    name = value;
    which = stringToEntityType[value];
    type = stringToBuildStepType[value];
}

BuildStepType BuildStep::getType()
{
    return this->type;
}

EntityType BuildStep::getWhich()
{
    return which;
}

string BuildStep::getName()
{
    return name;
}
