

#include <vector>
#include <random>
#include "GeneticOptimizer.hpp"
#include "Game.hpp"
#include "BuildOrder.hpp"
#include "BuildListTester.hpp"
#include <string.h>

vector<string> GeneticOptimizer::Terran_Entities = {
"refinery", "supply_depot", "barracks", "barrack_tech_lab", "barracks_reactor",
                "marauder", "reaper", "ghost", "ghost_academy", "bunker", "hellion", 
                "marine", "factory", "factory_tech_lab", "factory_reactor", "starport", 
                "starport_tech_lab", "starport_reactor", "raven", "banshee", "battlecruiser", 
                "fusion_core", "viking", "medivac", "siege_tank", "thor", "armory", "orbital_command", 
                "command_center", "scv", "engineering_bay", "planetary_fortress", "missile_turret", 
                "sensor_tower" 
};

GeneticOptimizer::GeneticOptimizer()
{
}

BuildOrder* GeneticOptimizer::createBuildList(char* entity) {
    
    vector<BuildStep*> steps;
    steps.push_back(new BuildStep("scv"));
        
    BuildOrder* result;
    result = new BuildOrder(steps);
    
    
    if(strcmp(entity, "siege_tank")==0) {
        vector<string> requirements {"refinery", "supply_depot", "barracks", "factory", "factory_tech_lab"};
        requirements.push_back(entity);
        vector<string> buildableEntities;
        string entry;
        std::default_random_engine generator;
        string race = "terran";
        for (unsigned int i=0; i < requirements.size(); i++) {
            buildableEntities = getBuildableEntities(result, race, entity);
            uniform_int_distribution<int> numberOfRandomEntries(1,buildableEntities.size());
            
            int rnd = numberOfRandomEntries(generator);
            // Create random number of random Entries for BuildList
            uniform_int_distribution<int> randomEntry(1, rnd);
            for(int y=0;y<rnd;y++) {
                int nextEntry = randomEntry(generator);
                result->addStepToBuildList(new BuildStep(buildableEntities[nextEntry]));
            }
            entry = requirements[i];
            
            result->addStepToBuildList(new BuildStep(requirements[i]));
        }
    }

    LOG_DEBUG("End createBuildList");
    return result;
}

vector<string> GeneticOptimizer::getBuildableEntities(BuildOrder* order, string &race, char* entity) {
    string strEntity = (string)entity;
    vector<string> listOfEntities;
    vector<string> result;
    if(race.compare("terran")==0)
        listOfEntities = this->Terran_Entities;
    for(unsigned int i = 0; i < listOfEntities.size();i++) {
        order->addStepToBuildList(new BuildStep(listOfEntities[i]));


//        if(order.doSanityCheck())
  //          result.push_back(listOfEntities[i]);
            // TODO: beautify!!!
            bool geht=true;
            try {
                order->doSanityCheck();
            } catch(...) { 
                geht=false;
            }
            if(geht) {
                
                result.push_back(listOfEntities[i]);
                if(listOfEntities[i] == strEntity)
                    return result;
            }



        order->removeLastStepFromBuildList();
    }   
    return result;
}

void GeneticOptimizer::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("GeneticOptimizer Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);

    if( (strcmp(entity, "siege_tank")==0) ) {
        
        
        LOG_DEBUG("start");

        // Create BuildOrder; add entity to avoid empty-Buildlist-nagging by doSanityCheck
        
        vector<BuildStep*> steps;
        steps.push_back(new BuildStep("scv"));
            BuildOrder* buildOrder;

        buildOrder = createBuildList(entity);
        
        // Start algorithm here
        
        vector<BuildStep*> temp;
        temp = buildOrder->getBuildList();
        for(int i = 0; i<temp.size(); i++)
            LOG_DEBUG(temp[i]->getName());
        

       
      //unsigned long fitness = Game::getFitnessPush(*buildOrder);
     //LOG_DEBUG("Fitness of Buildlist: " << fitness);
        
    } else {
        LOG_DEBUG("nix wars");
    }
}

