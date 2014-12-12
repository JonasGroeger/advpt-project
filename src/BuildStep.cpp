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
