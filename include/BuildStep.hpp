#pragma once
#include "Entity.hpp"

#include <map>
#include <string>

using namespace std;

enum class BuildStepType { UPGRADE, PRODUCE, CONSTRUCT, CHRONO_BOOST };

class BuildStep
{
private:
    BuildStepType type;
    EntityType which;
    string name;
public:
    static map<string, EntityType> stringToEntityType;
    // DEBUGGING
    static map<EntityType, string> entityTypeToString;
    static void initMap(void)
    {
        for (auto it = BuildStep::stringToEntityType.begin(); it != BuildStep::stringToEntityType.end(); it++)
        {
            BuildStep::entityTypeToString[it->second] = it->first;
        }
    }

    BuildStep(string value);

    BuildStepType getType();
    EntityType getWhich();

    //Just for testing
    string getName();
};
