#pragma once

#include "Entity.hpp"

#include <map>
#include <string>
#include <iostream>

using namespace std;

enum class BuildStepType
{
    UPGRADE, PRODUCE, CHRONO_BOOST
};

class BuildStep
{
    // Only BuildStepPool should be allowed to create instances of this class
    friend class BuildStepPool;
private:
    BuildStep(string value);
    BuildStep(const BuildStep& copy);
    BuildStep& operator=(BuildStep& copy);
    ~BuildStep() = default;
    
    // Private members
    BuildStepType buildstepType;
    EntityType entityType;
    string name;

public:
    // Getters for private members
    BuildStepType getBuildStepType(); 
    EntityType getEntityType(); 
    string getName();

    static map<string, EntityType> stringToEntityType;
    static map<string, BuildStepType> stringToBuildStepType;
    
    // Initialize the reverse maps
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
};

/*
 * BuildStepPool is a Singleton Class that is used to get Pointers to BuildStep Instances
 * These pointers should NEVER be freed, BuildStepPool will take care of alle this internally
 */
class BuildStepPool
{
    private:
        map<string, BuildStep*> stringToBuildStep;
        // Private Constructor
        BuildStepPool();
        // Stop the compiler generating methods of copy the object
        BuildStepPool(BuildStepPool const& copy);            // Not Implemented
        BuildStepPool& operator=(BuildStepPool const& copy); // Not Implemented
        
    public:
        static BuildStepPool& getInstance();
        BuildStep *getBuildStep(string);
            
};
