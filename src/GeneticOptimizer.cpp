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
"refinery", "supply_depot", "barracks", "barracks_tech_lab", "barracks_reactor",
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
    {"barracks_tech_lab","barracks"}, 
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

/*
 * Creates random buildlist that produces a @entity 
 */
BuildOrder* GeneticOptimizer::createBuildList(char* entity) {
    string strEntity = entity;
        
    BuildOrder* result;
    cout << "Create new BuildOrder: " << endl;
    result = new BuildOrder();
    
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
            
            int rnd = rand() % getConfigInteger("Genetic", "NumberStepsBetween", 5);
            // Create random number of random Entries for BuildList
            
            for(int y=0;y<rnd;y++) {
                int nextEntry = rand() % buildableEntities.size();
                nextEntity = buildableEntities[nextEntry];

                if(alreadyAddedEntities.count(nextEntity)==0) {
                    alreadyAddedEntities[nextEntity] = 1;
                } else {
                    alreadyAddedEntities[nextEntity] ++;
                }

                // check, if entity has prerequisites
                if(entitiesWithPrerequisites.count(nextEntity)!=0) {
                    const string& prerequisite = entitiesWithPrerequisites[nextEntity];

                    if (alreadyAddedEntities[prerequisite] > 0)
                    {
                        result->addBuildStep(nextEntity);
                        alreadyAddedEntities[prerequisite] --;
                    }
                } else {
                    result->addBuildStep(nextEntity);
                }
            }
            
            result->addBuildStep(requirements[i]);
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
        order->addBuildStep(listOfEntities[i]);

        if(order->isPossible() && listOfEntities[i] != strEntity)
        {
                result.push_back(listOfEntities[i]);
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
        buildLists[i].second->print();
        unsigned long fitness=0;

        if ((buildLists[i].second)->getBuildList().size() == 0)
        {
                fitness = LONG_MAX;
        }
        else
        {
                fitness = Game::getFitnessPush(*buildLists[i].second);            
        }
        buildLists[i].first = fitness;
    }

    std::sort(buildLists.begin(), buildLists.end());
}


bool flipCoin() {
    int i = rand()%2;
    return (i==0);
}

/*
 * @buildLists are rated and sorted (ascending by fitness) buildLists (must have at least 2 entries)
 * @entity target entity
 */
void GeneticOptimizer::mutateBuildLists(vector<pair<unsigned long, BuildOrder*>> &buildLists, char* entity) {
    vector<BuildStep*> mumSteps = buildLists[0].second->buildSteps;
    vector<BuildStep*> dadSteps = buildLists[1].second->buildSteps;

    // Configuration for Mutation-Step
    double maxProbability = getConfigDouble("Genetic", "MaxMutationProbability", 0.3);
    double probabilityFraction = ((double)1)/buildLists.size();
 
    double currentProbabilityFraction = probabilityFraction;
    double currentProbability = 0;
    BuildStepPool& buildStepPool = BuildStepPool::getInstance();

    string race = "terran";

    // Take first two Buildlists and replace positions 3-end with Mutant-Children
    for(unsigned int i = 2;i<buildLists.size();i++) {
        BuildOrder* child;
        child = new BuildOrder();

        // recombine
        do {
            child->clearBuildSteps();
            bool containsEntity = false;

            auto mumIterator = mumSteps.begin();
            auto dadIterator = dadSteps.begin();

            while (mumIterator != mumSteps.end() && dadIterator != dadSteps.end())
            {
                BuildStep *chosenStep = (rand() % 2 == 0) ? *mumIterator : *dadIterator;
                mumIterator ++;
                dadIterator ++;

                child->addBuildStep(chosenStep);

                if (chosenStep->getName() == entity)
                {
                    containsEntity = true;
                }
            }

            if (!containsEntity) 
            {
                while (mumIterator != mumSteps.end()) 
                {
                    child->addBuildStep(*mumIterator);
                    mumIterator ++;
                }

                while (dadIterator != dadSteps.end())
                {
                    child->addBuildStep(*dadIterator);
                    dadIterator ++;
                }
            }

            /*
            for(unsigned int y = 0; y<min(mumSteps.size(), dadSteps.size());y++) {

                BuildStep* chosenStep;
                chosenStep = (rand()%2==0)?mumSteps[y]:dadSteps[y];

                child->addBuildStep(chosenStep);
                if (chosenStep->getName() == entity)
                {
                    containsEntity = true;
                    break;
                }
            }
            */

        } while(!child->isPossible());

        // mutate
        vector<BuildStep*> originalBuildList = child->buildSteps;
        child->print();
        BuildOrder* mutatedChild = new BuildOrder();

        // Calculate Probability of mutation in this list
        currentProbability = maxProbability*probabilityFraction*((double)(i+1));

        for(unsigned int currentStep = 0; currentStep <child->buildSteps.size()-1; currentStep++) {
            // Decide, if we do something with the current BuildStep
            double val = (double)rand() / RAND_MAX;
            
            if(val<currentProbability) {
                // Decide, what to to
                int what = rand()%3;
                int rnd = 0;
                vector<string> buildableEntities;
                switch(what) {
                    case 0:
                        cout << "delete a step" << endl;
                        // delete step (don't add it)... except it's the first step
                        if(currentStep ==0)
                            mutatedChild->buildSteps.push_back(child->buildSteps[currentStep]);
                        break;
                    case 1:
                        // add new step
                        cout << "add a step" << endl;
                        buildableEntities = 
                            GeneticOptimizer::getBuildableEntities(mutatedChild, race, entity);
                        
                        if(buildableEntities.size()>0) {
                            rnd = rand()%buildableEntities.size();
                            mutatedChild->buildSteps.push_back(buildStepPool.getBuildStep(buildableEntities[rnd]));
                            mutatedChild->buildSteps.push_back(child->buildSteps[currentStep]);
                            
                        }
                        break;
                    case 2: 
                        // change step
                        cout << "change a step" << endl;
                        buildableEntities = 
                            GeneticOptimizer::getBuildableEntities(mutatedChild, race, entity);
                        
                        if(buildableEntities.size()>0) {
                            rnd = rand()%buildableEntities.size();

                            mutatedChild->buildSteps.push_back(buildStepPool.getBuildStep(buildableEntities[rnd]));
                            if(!mutatedChild->isPossible())
                                mutatedChild->removeLastStepFromBuildList();
                        }
                        break;
                }

            } else {
                mutatedChild->buildSteps.push_back(child->buildSteps[currentStep]);
            }
            currentProbabilityFraction += probabilityFraction;
        }
        mutatedChild->buildSteps.push_back(buildStepPool.getBuildStep(entity));
        delete buildLists[i].second;
        buildLists[i].second = mutatedChild;
        
    }
    cout << "End of Mutation" << endl;
}

void GeneticOptimizer::run(char *entity, char *mode, int maxSimulationTime)
{
    LOG_DEBUG("GeneticOptimizer Algorithm started with entity: \"" << entity << "\" mode: \"" << mode << "\" maxSimulationTime: " << maxSimulationTime);
    int numberOfGenerations = getConfigInteger("Genetic", "NumberOfGenerations", 20);
    numberOfGenerations = 50;
    int numberOfLists = getConfigInteger("Genetic", "NumberOfLists", 20);

    if( (strcmp(entity, "siege_tank")==0) ) {
        // Create initial BuildOrder; 
        std::vector<pair<unsigned long, BuildOrder*>> *listOfBuildlists = generateRandomBuildLists(numberOfLists, entity);
     
        // Start algorithm here
        

        for(int nog=0;nog<numberOfGenerations;nog++) {
            rateBuildLists(*listOfBuildlists);
            mutateBuildLists(*listOfBuildlists, entity);
        }
        rateBuildLists(*listOfBuildlists);
        mutateBuildLists(*listOfBuildlists, entity);

        for(unsigned int i = 0; i<listOfBuildlists->size();i++)
        {
            cout << "---------------------------------------------------------------" << endl;
            cout << "Liste Nr: " << (i+1) << " Fitness: " << (*listOfBuildlists)[i].first << endl;
            (*listOfBuildlists)[i].second->print();
        }

    } else {
        LOG_DEBUG("nix wars");
    }
}

