
#include <vector>
#include <random>
#include "GeneticOptimizer.hpp"
#include "Game.hpp"
#include "BuildOrder.hpp"
#include "BuildStep.hpp"
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
    // Initialize Random-Generator
    srand(time(NULL));
}


map<EntityType, string> GeneticOptimizer::getEntityTypeToString() {
    map<string, EntityType>::iterator it = BuildStep::stringToEntityType.begin();
    map<EntityType, string> result;
    while(it!=BuildStep::stringToEntityType.end()) {
        result.insert(pair<EntityType, string>(it->second, it->first));
        ++it;
    }
    return result;
}

vector<string> GeneticOptimizer::getDependencyList(string entity) {


    vector<string> dependencies;
    vector<EntityType> todoList;
    EntityType type = BuildStep::stringToEntityType[entity];
    map<EntityType, bool> alreadyDone;
    map<EntityType, string> entityTypeToString = getEntityTypeToString();

    todoList.push_back(type);
    map<string, int> alreadyAddedToDependencyList;
   
    do {
        // Get first element and delete from todoList
        EntityType currentType = todoList[0];
        todoList.erase(todoList.begin());
        
        // Get Dependencies
        vector<EntityType> subtype = BuildOrder::dependencies[currentType];
        
        
        // Walk through all Dependencies
        for(unsigned int y = 0; y<subtype.size();y++) {
            // Check, if EntityType already processed (prevents circular dependencies resulting in infinite loops)
            EntityType currentSubType = subtype[y];
            // Add to todolist, if not already done
            if(alreadyDone.find(currentSubType)==alreadyDone.end()){
                // make sure, we process the element
                todoList.push_back(currentSubType);
            }
                
            string EntityName = entityTypeToString.find(currentSubType)->second;
            // If already in list...
            if(alreadyAddedToDependencyList.find(EntityName)!=alreadyAddedToDependencyList.end()) {

                // remove
                int index = alreadyAddedToDependencyList.find(EntityName)->second;
           //     cout << "entfernen an position" << index << endl;
                dependencies.erase(dependencies.begin() + index);
                alreadyAddedToDependencyList.erase(
                    alreadyAddedToDependencyList.find(EntityName)
                    );
            } 
            // Add it to dependency-list
            dependencies.push_back(EntityName);
            alreadyAddedToDependencyList.insert(pair<string, int>(EntityName, dependencies.size()-1));

            // Mark as already done
            alreadyDone.insert(pair<EntityType, bool>(currentSubType, true));
        
        }
        
    } while(todoList.size()>0);
    
    // Inverse order of dependencies
    vector<string> inverseDependencies;
    for(int i = dependencies.size()-1;i>=0;i--)
        inverseDependencies.push_back(dependencies[i]);

    return inverseDependencies;
}

BuildOrder* GeneticOptimizer::createBuildList(char* entity) {
    string strEntity = entity;
    vector<BuildStep*> steps;
    steps.push_back(new BuildStep("scv"));
        
    BuildOrder* result;
    cout << "Create new BuildOrder: " << endl;
    result = new BuildOrder(steps);
    result->clearBuildSteps();
    
    map<string, int> alreadyAddedEntities;
    if(strcmp(entity, "siege_tank")==0) {
        //vector<string> requirements {"refinery", "supply_depot", "barracks", "factory", "factory_tech_lab"};
        vector<string> requirements = getDependencyList(strEntity);
        //cout << "Listenstart" << endl;
       // for(int i = 0; i<requirements.size(); i++)
       //     cout << requirements[i] << endl;

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
    cout << "Buildliste fertig" << endl;
    return result;
}

vector<string> GeneticOptimizer::getBuildableEntities(BuildOrder* order, string &race, char* entity) {
    string strEntity = (string)entity;
    vector<string> listOfEntities;
    vector<string> result;
    if(race.compare("terran")==0)
        listOfEntities = this->Terran_Entities;
    else if(race.compare("protoss"))
        listOfEntities = this->Protoss_Entities;
    else
        listOfEntities = this->Zerg_Entities;

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

void GeneticOptimizer::rateBuildLists(vector<pair<unsigned long, BuildOrder*>> &buildLists) {
    
    for(unsigned int i = 0; i<buildLists.size();i++)
    {

        unsigned long fitness=0;
        
        try {
            fitness = Game::getFitnessPush(*buildLists[i].second);            
            buildLists[i].first = fitness;
        } catch(...) {
            fitness = INT_MAX;
        }
        //buildLists[i].first = 100;
    }
    std::sort(buildLists.begin(), buildLists.end());
    
    
}


bool flipCoin() {
    int i = rand()%2;
    return (i==0);
}

void GeneticOptimizer::mutateBuildLists(vector<pair<unsigned long, BuildOrder*>> &buildLists) {
    vector<BuildStep*> mumSteps = buildLists[0].second->buildSteps;
    vector<BuildStep*> dadSteps = buildLists[1].second->buildSteps;
    
    vector<BuildStep*> steps;
    steps.push_back(new BuildStep("scv"));
        
    

    // Take first two Buildlists and replace positions 3-end with Mutant-Children
    for(unsigned int i = 2;i<buildLists.size();i++) {
        BuildOrder* child;
        child = new BuildOrder(steps);
        child->clearBuildSteps();

        // recombine
        bool geht=true;
            
     //   do {
            child->clearBuildSteps();
            for(unsigned int y = 0; y<min(mumSteps.size(), dadSteps.size());y++) {
            
                BuildStep* chosenStep;
                chosenStep = (rand()%2==0)?mumSteps[y]:dadSteps[y];

                child->addStepToBuildList(
                    chosenStep
                );

            }
       /*     try {
                geht = true;
                child->doSanityCheck();
            } catch(...) { 
                
                geht=false;
            }
        } while(!geht);
        */
        // mutate
        buildLists[i].second = child;
    }
}

void GeneticOptimizer::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("GeneticOptimizer Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);
    int numberOfGenerations = getConfigInteger("Genetic", "NumberOfGenerations", 10);
    int numberOfLists = getConfigInteger("Genetic", "NumberOfLists", 20);
    
    if( (strcmp(entity, "siege_tank")==0) ) {
        
        // Create initial BuildOrder; 
        std::vector<pair<unsigned long, BuildOrder*>> *listOfBuildlists = generateRandomBuildLists(numberOfLists, entity);
            
        rateBuildLists(*listOfBuildlists);
        

        // Start algorithm here
       /* for(int nog=0;nog<numberOfGenerations;nog++) {
           
            rateBuildLists(*listOfBuildlists);
            mutateBuildLists(*listOfBuildlists);
        }*/
       
        for(unsigned int i = 0; i<listOfBuildlists->size();i++)
        {
           cout << "Liste Nr: " << (i+1) << " Fitness: " << (*listOfBuildlists)[i].first << endl;
        }
        
    } else {
        LOG_DEBUG("nix wars");
    }
}

