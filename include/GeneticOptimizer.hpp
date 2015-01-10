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
public:
    GeneticOptimizer();
    static vector<string> Terran_Entities;
    void run(char *entity, char *mode, int maxSimulationTime);

};
