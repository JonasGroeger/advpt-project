#include "ConfigParser.h"

void ConfigParser::parseConfig(char *file)
{
    XMLError load_result = xmlConfig.LoadFile(file);
    if (load_result != XML_SUCCESS)
    {
        throw std::invalid_argument("Malformed configuration file.");
    }

    XMLNode *rootNode = xmlConfig.RootElement();
    if (rootNode == nullptr)
    {
        throw std::invalid_argument("Malformed configuration file: No root element found.");
    }

    LOG_DEBUG("Parsing races");
    // Iterate through each race
    for (XMLElement *race = rootNode->FirstChildElement(NODE_RACE); race != nullptr;
         race = race->NextSiblingElement(NODE_RACE))
    {
        //the race struct
        Race currRace;
        currRace.name = race->Attribute(ATTRIBUTE_NAME);
        LOG_DEBUG(" Parsing race ["<<currRace.name<<"]");
        //this map will hold all actions by this race
        map<action_t, BuildAction> actions;

        LOG_DEBUG("  Parsing actions");
        // parse the actions and put them to the map
        for (XMLElement *action = race->FirstChildElement(NODE_ACTION); action != nullptr;
             action = action->NextSiblingElement(NODE_ACTION))
        {
            BuildAction buildAction;
            buildAction.name = action->Attribute(ATTRIBUTE_NAME);
            LOG_DEBUG("   Parsing action ["<<buildAction.name<<"]");
            buildAction.id = getUnitId(buildAction.name);
            LOG_DEBUG("    Unit [" << buildAction.name << "] added with id [" << buildAction.id <<"]");

            //costs of the action
            XMLElement *costs = action->FirstChildElement(NODE_COSTS);
            buildAction.cost.gas = stoi(costs->Attribute(ATTRIBUTE_GAS));
            buildAction.cost.minerals = stoi(costs->Attribute(ATTRIBUTE_MINERALS));
            buildAction.cost.supply = stoi(costs->Attribute(ATTRIBUTE_SUPPLY));
            buildAction.cost.time = stoi(costs->Attribute(ATTRIBUTE_TIME));
            //if this action is consuming other units
            addUnitsToVector(costs, NODE_UNIT, buildAction.cost.units);

            //borrows
            XMLElement *borrows = action->FirstChildElement(NODE_BORROWS);
            addUnitsToVector(borrows, NODE_UNIT, buildAction.borrows);

            //dependencies
            XMLElement *dependencies = action->FirstChildElement(NODE_DEPENDENCIES);
            addUnitsToVector(dependencies, NODE_UNIT, buildAction.dependencies);

            //results
            XMLElement *results = action->FirstChildElement(NODE_RESULTS);

            buildAction.result.minerals = stoi(results->Attribute(ATTRIBUTE_MINERALS));
            buildAction.result.gas = stoi(results->Attribute(ATTRIBUTE_GAS));
            buildAction.result.supply = stoi(results->Attribute(ATTRIBUTE_SUPPLY));
            addUnitsToVector(results, NODE_UNIT, buildAction.result.units);

            actions[buildAction.id] = buildAction;
        }
        currRace.actions = actions;

        LOG_DEBUG("  Parsing maxUnits");
        // Maximum unit numbers
        XMLElement *maxElement = race->FirstChildElement(NODE_MAX_UNITS);
        for (XMLElement *max = maxElement->FirstChildElement(NODE_UNIT); max != nullptr;
             max = max->NextSiblingElement(NODE_UNIT))
        {
            // TODO: Insert maximum unit number in race struct
            // int max_number = stoi(max->Attribute(ATTRIBUTE_MAX));
        }

        LOG_DEBUG("  Parsing workers");
        // Workers for this race
        XMLElement *workerElement = race->FirstChildElement(NODE_WORKER);
        bool bWorkerFound = false;
        for (XMLElement *worker = workerElement->FirstChildElement(); worker != nullptr;
             worker = worker->NextSiblingElement(NODE_UNIT))
        {
            string workerName = worker->Attribute(ATTRIBUTE_NAME);
            auto it = find_if(currRace.actions.begin(), currRace.actions.end(),
                    [&workerName](const std::pair<action_t, BuildAction> &action)
                    {
                        return workerName.compare(action.second.name) == 0;
                    });
            if (it != currRace.actions.end())
            {
                (*it).second.isWorker = true;
                bWorkerFound = true;
                LOG_DEBUG("   Found worker ["<<worker->Attribute(ATTRIBUTE_NAME)<<"]");
            }
        }
        if (!bWorkerFound)
        {
            throw std::out_of_range("No worker was found!");
        }


        // Gas harvesters for this race
        //gasharvesterId is used later to put the gas harvester as a dependency to each unit that has gas costs
        LOG_DEBUG("  Parsing Gas Harvesters");
        action_t gasHarvesterId = -1;
        XMLElement *gas_harvesters = race->FirstChildElement(NODE_GAS_HARVESTER);
        bool bGasHarvesterFound = false;
        for (XMLElement *gas_element = gas_harvesters->FirstChildElement(); gas_element != nullptr;
             gas_element = gas_element->NextSiblingElement(NODE_UNIT))
        {
            string gasHarvesterName = gas_element->Attribute(ATTRIBUTE_NAME);
            auto it = find_if(currRace.actions.begin(), currRace.actions.end(),
                    [&gasHarvesterName](const std::pair<action_t, BuildAction> &action)
                    {
                        return gasHarvesterName.compare(action.second.name) == 0;
                    });
            if (it != currRace.actions.end())
            {
                (*it).second.isGasHarvester = true;
                gasHarvesterId = (*it).second.id;
                bGasHarvesterFound = true;
                LOG_DEBUG("   Found gas harvester ["<<gasHarvesterName<<"]");
            }
        }
        if (!bGasHarvesterFound)
        {
            throw std::out_of_range("No gas harvester was found for race [" + currRace.name +"]");
        }

        //get the default supply for this race
        LOG_DEBUG("  Parsing default supply");
        string sDefSupply = race->Attribute(ATTRIBUTE_DEFAULT_SUPPLY);
        auto it = std::find_if(currRace.actions.begin(), currRace.actions.end(),
                                [&sDefSupply](const std::pair<action_t, BuildAction> &action)
                                {
                                    return sDefSupply.compare(action.second.name) == 0;
                                });
        if(it == currRace.actions.end())
        {
            throw std::out_of_range("No default supply building found!");
        }
        else
        {
            LOG_DEBUG("   Found default supply ["<<(*it).second.name<<"]");
            currRace.defaultSupplyAction = (*it).second.id;
        }

        //parse our start_config
        LOG_DEBUG("  Parsing start_config");
        XMLElement* startConfig = race->FirstChildElement(NODE_START_UNITS);
        map<action_t, int> startMap;
        for(XMLElement* startUnit = startConfig->FirstChildElement(NODE_UNIT);
            startUnit != nullptr;
            startUnit = startUnit->NextSiblingElement(NODE_UNIT))
        {
            string startUnitName = startUnit->Attribute(ATTRIBUTE_NAME);
            int startCount = startUnit->IntAttribute(ATTRIBUTE_COUNT);

            auto it = std::find_if(currRace.actions.begin(), currRace.actions.end(),
                    [&startUnitName](const std::pair<action_t, BuildAction> &action)
                    {
                        return startUnitName.compare(action.second.name) == 0;
                    });
            if(it == currRace.actions.end())
            {
                throw std::out_of_range("The Unit ["+ startUnitName + "] of race ["+ currRace.name+"is not available in our actions!");
            }
            else
            {
                LOG_DEBUG("   Found start unit ["<<(*it).second.name<<"] with count [" << startCount << "]");
                startMap[(*it).second.id] = startCount;
            }
        }
        currRace.startUnits = startMap;

        //resolve the gas dependencies
        LOG_DEBUG("  Parsing units with gas dependencies");
        for(auto it = currRace.actions.begin(); it != currRace.actions.end(); ++it)
        {
            if((*it).second.cost.gas > 0)
            {
                LOG_DEBUG("   Found unit [" << (*it).second.name << "] with gas dependency");
                (*it).second.dependencies.push_back(std::pair<action_t, int>(gasHarvesterId, 1));
            }
        }
        //the races map will hold all available races and their corresponding struct
        races[currRace.name] = currRace;
    }
}

const BuildAction& ConfigParser::getDefaulSupplyAction()
{
    return currentRace.actions[currentRace.defaultSupplyAction];
}

const map<action_t, int> ConfigParser::getStartConfig()
{
    return currentRace.startUnits;
}

const vector<BuildAction> ConfigParser::getAllActions()
{
    vector<BuildAction> resultVec;
    for (auto pair : buildActionMap)
    {
        resultVec.push_back(pair.second);
    }
    return resultVec;
}

const BuildAction &ConfigParser::getAction(string actionName)
{
    bool bFound = false;
    action_t actionId;
    for (auto race : races)
    {
        auto it = find_if(race.second.actions.begin(), race.second.actions.end(),
                [&actionName,&actionId](const std::pair<action_t, BuildAction> &entry)
                {
                    //save the actionId here, when search is successfull we need that value
                    actionId = entry.second.id;
                    return entry.second.name.compare(actionName) == 0;
                }
        );

        if (it != race.second.actions.end())
        {
            currentRace = race.second;
            bFound = true;
            LOG_DEBUG("[ConfigParser::getAction(string) : Found race ["<<currentRace.name<<"] for action ["<< actionName << "]");
            break;
        }
    }
    if (!bFound)
    {
        throw std::out_of_range("Unable to find: " + actionName);
    }
    return currentRace.actions[actionId];
}

const BuildAction &ConfigParser::getAction(action_t id)
{
    if (currentRace.actions.count(id) == 0)
    {
        throw std::out_of_range("Unable to find: " + std::to_string(id));
    }
    else
    {
        return currentRace.actions[id];
    }
}

long ConfigParser::getActionCount()
{
    return currentRace.actions.size();
}


action_t ConfigParser::getUnitId(string unitName)
{
    if (unitMap.count(unitName) == 0)
    {
        unitMap.insert(std::pair<string, int>(unitName, unitCount));
        ++unitCount;
    }
    return unitMap[unitName];
}

void ConfigParser::addUnitsToVector(XMLElement *element, const char *node, vector<std::pair<action_t, int>> &targetVector)
{
    for (XMLElement *tmpElement = element->FirstChildElement(node); tmpElement != nullptr; tmpElement = tmpElement->NextSiblingElement())
    {
        string unitName = tmpElement->Attribute(ATTRIBUTE_NAME);
        action_t id = getUnitId(unitName);

        std::vector<std::pair<action_t, int>>::iterator it;
        it = std::find_if(targetVector.begin(), targetVector.end(), [&id](std::pair<action_t, int> &pair)
        {
            return (pair.first == id);
        });

        if (it != targetVector.end())
        {
            (*it).second++;
        }
        else
        {
            targetVector.push_back(std::pair<action_t, int>(unitMap[unitName], 1));
        }
    }
}

