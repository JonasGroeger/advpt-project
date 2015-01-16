#pragma once

#include <vector>
#include "BuildStep.hpp"
#include "BuildOrder.hpp"
#include "Debug.hpp"

class GeneticOptimizer
{
private:
    BuildOrder* createBuildList(char* entity);
    vector<string> getBuildableEntities(BuildOrder* order, string &race, char* entity);
    
    vector<pair<unsigned long, BuildOrder*>>* generateRandomBuildLists(int numberOfBuildLists, char* entity);
    void rateBuildLists(vector<pair<unsigned long, BuildOrder*>> &buildLists);
    void mutateBuildLists(vector<pair<unsigned long, BuildOrder*>> &buildLists);
    vector<string> getDependencyList(string entity);
    map<EntityType, string> getEntityTypeToString();
public:
    GeneticOptimizer();
    static vector<string> Terran_Entities;
    static vector<string> Zerg_Entities;
    static vector<string> Protoss_Entities;
    void run(char *entity, char *mode, int maxSimulationTime);

};