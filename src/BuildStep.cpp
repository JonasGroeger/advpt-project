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
    {"mothership", PROTOSS_MOTHERSHIP}
};
map<EntityType, string> BuildStep::entityTypeToString;

BuildStep::BuildStep(string value)
{
    name = value;
    which = stringToEntityType[value];
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
