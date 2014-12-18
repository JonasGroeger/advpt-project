#pragma once
#include "Entity.hpp"

#include <map>
#include <string>

using namespace std;

enum class BuildStepType { UPGRADE, PRODUCE, CONSTRUCT, CHRONO_BOOST };

class BuildStep
{
private:
    BuildStepType buildstepType;
    EntityType entityType;
    string name;
public:
    static map<string, EntityType> stringToEntityType;
    static map<string, BuildStepType> stringToBuildStepType;
    // DEBUGGING
    static map<EntityType, string> entityTypeToString;
    static map<BuildStepType, string> buildStepTypeToString;
    static void initMap(void)
    {
        for (auto it = BuildStep::stringToEntityType.begin(); it != BuildStep::stringToEntityType.end(); it++)
        {
            BuildStep::entityTypeToString[it->second] = it->first;
        }
        for (auto it = BuildStep::stringToBuildStepType.begin(); it != BuildStep::stringToBuildStepType.end(); it++)
        {
            BuildStep::buildStepTypeToString[it->second] = it->first;
        }
    }

    BuildStep(string value);

    BuildStepType getBuildStepType();
    EntityType getEntityType();

    //Just for testing
    string getName();
};
