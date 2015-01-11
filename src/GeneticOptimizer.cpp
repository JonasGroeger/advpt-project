
#include <vector>
#include <random>
#include "GeneticOptimizer.hpp"
#include "Game.hpp"
#include "BuildOrder.hpp"
#include "BuildListTester.hpp"
#include <string.h>
#include <utility>
#include <time.h>
#include <stdlib.h>
#include <climits>

vector<string> GeneticOptimizer::Terran_Entities = {
"refinery", "supply_depot", "barracks", "barrack_tech_lab", "barracks_reactor",
                "marauder", "reaper", "ghost", "ghost_academy", "bunker", "hellion", 
                "marine", "factory", "factory_tech_lab", "factory_reactor", "starport", 
                "starport_tech_lab", "starport_reactor", "raven", "banshee", "battlecruiser", 
                "fusion_core", "viking", "medivac", "siege_tank", "thor", "armory", "orbital_command", 
                "command_center", "scv", "engineering_bay", "planetary_fortress", "missile_turret", 
                "sensor_tower" 
};

vector<string> GeneticOptimizer::Protoss_Entities =   {
    "assimilator","forge", "pylon", "nexus", "photon_cannon", "phoenix", "void_ray", "carrier", 
                "fleet_beacon","dark_shrine", "templar_archives", "robotics_bay", "colossus", 
                "observer", "immortal", "warp_prism", "sentry", "stalker", "zealot", 
                "probe", "gateway", "cybernetics_core", "robotics_facility", "high_templar", 
                "dark_templar", "twilight_council", "stargate", "archon", "mothership"};


vector<string> GeneticOptimizer::Zerg_Entities = {
    "hatchery", "evolution_chamber", "crawler", "drone", "queen", "lair", "overseer", "overlord", 
                "spawning_pool", "spine_crawler", "roach_warren", "baneling_nest", "extractor", 
                "zergling", "roach", "baneling", "infestation_pit", "infestor", "hive", 
                "spire", "greater_spire", "corruptor", "mutalisk", "brood_lord", "ultralisk_cavern", 
                "ultralisk", "nydus_worm","nydus_network", "hydralisk_den", "hydralisk" };


// TODO: add Zerg and Protoss
std::map<string, string> entitiesWithPrerequisites = {
    {"planetary_fortress", "command_center"}, 
    {"orbital_command", "command_center"},
    {"starport_reactor", "starport"},
    {"factory_reactor","factory"}, 
    {"starport_tech_lab","starport"}, 
    {"barrack_tech_lab","barracks"}, 
    {"barracks_reactor", "barracks"}, 
    {"factory_tech_lab","factory"} 
};

GeneticOptimizer::GeneticOptimizer()
{
    srand(time(NULL));
}

BuildOrder* GeneticOptimizer::createBuildList(char* entity) {
    
    vector<BuildStep*> steps;
    steps.push_back(new BuildStep("scv"));
        
    BuildOrder* result;
    result = new BuildOrder(steps);
    
    map<string, int> alreadyAddedEntities;
    if(strcmp(entity, "siege_tank")==0) {
        vector<string> requirements {"refinery", "supply_depot", "barracks", "factory", "factory_tech_lab"};
        requirements.push_back(entity);
        vector<string> buildableEntities;
        string entry;
        string nextEntity;
        string race = "terran";
        for (unsigned int i=0; i < requirements.size(); i++) {
            buildableEntities = getBuildableEntities(result, race, entity);
            
            int rnd = rand()%buildableEntities.size();
            // Create random number of random Entries for BuildList
            
            for(int y=0;y<rnd;y++) {
                int nextEntry = rand()%rnd;
                nextEntity = buildableEntities[nextEntry];

                if(alreadyAddedEntities.count(nextEntity)==0) {
                    alreadyAddedEntities.insert(pair<string, int>(nextEntity, 1));
                } else {
                    alreadyAddedEntities[buildableEntities[nextEntry]]++;
                }
                // check, if entity has prerequisites
                if(entitiesWithPrerequisites.count(nextEntity)!=0) {
                    int count = alreadyAddedEntities[nextEntity];
                    if(count>0) {
                        result->addStepToBuildList(new BuildStep(nextEntity));
                        count--;
                        alreadyAddedEntities[nextEntity] = count;
                    }
                } else {
                    result->addStepToBuildList(new BuildStep(nextEntity));
                }
            }
            
            
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

vector<pair<unsigned long, BuildOrder*>>* GeneticOptimizer::generateRandomBuildLists(int numberOfBuildLists, char* entity) {
    vector<pair<unsigned long, BuildOrder*>>* listOfBuildlists = new vector<pair<unsigned long, BuildOrder*>>();

    for(int i = 0; i<numberOfBuildLists; i++) {
        pair<unsigned long, BuildOrder*> newPair(0, createBuildList(entity));
        listOfBuildlists->push_back(newPair);
    }
    return listOfBuildlists;
}

void GeneticOptimizer::rateBuildLists(vector<pair<unsigned long, BuildOrder*>> buildLists) {
    
    for(unsigned int i = 0; i<buildLists.size();i++)
    {
        unsigned long fitness=0;
        
        
        
        try {
            
            fitness = Game::getFitnessPush(*buildLists[i].second);
            
            buildLists[i].first = fitness;
        } catch(...) {
            fitness = INT_MAX;
        }
        
    }
    std::sort(buildLists.begin(), buildLists.end());
    
}

void GeneticOptimizer::mutateBuildLists(vector<pair<unsigned long, BuildOrder*>>) {
    
}

void GeneticOptimizer::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("GeneticOptimizer Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);
    int numberOfGenerations = 10;
    int numberOfLists=10;
    
    if( (strcmp(entity, "siege_tank")==0) ) {
        
        // Create initial BuildOrder; 
        std::vector<pair<unsigned long, BuildOrder*>> *listOfBuildlists = generateRandomBuildLists(numberOfLists, entity);
        

        // Switch cout to gameResultBuffer to suppress output
        std::stringstream gameResultBuffer;
        std::streambuf *originalBuffer = std::cout.rdbuf();
        std::cout.rdbuf(gameResultBuffer.rdbuf());

        // Start algorithm here
        for(int nog=0;nog<numberOfGenerations;nog++) {
            rateBuildLists(*listOfBuildlists);
            mutateBuildLists(*listOfBuildlists);

        }
        // Restore cout
        std::cout.rdbuf(originalBuffer);

       // cout << "finished algorithm";
        // Print result
        for(unsigned int i = 0; i<listOfBuildlists->size();i++)
        {
           // cout << "Liste Nr: " << (i+1) << "Fitness: " << (*listOfBuildlists)[i].first << endl;
        }
        
    } else {
        LOG_DEBUG("nix wars");
    }
}

