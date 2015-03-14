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

    // Iterate through found races
    for (XMLElement *race = rootNode->FirstChildElement(NODE_RACE); race != nullptr;
         race = race->NextSiblingElement(NODE_RACE))
    {
        Race currRace;
        currRace.name = race->Attribute(ATTRIBUTE_NAME);
        map<action_t, BuildAction> actions;

        // parse the actions
        for (XMLElement *action = race->FirstChildElement(NODE_ACTION); action != nullptr;
             action = action->NextSiblingElement(NODE_ACTION))
        {
            BuildAction buildAction;
            buildAction.name = action->Attribute(ATTRIBUTE_NAME);
            buildAction.id = getUnitId(buildAction.name);

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
        races[currRace.name] = currRace;

        // Maximum unit numbers
        XMLElement *maxElement = race->FirstChildElement(NODE_MAX_UNITS);
        for (XMLElement *max = maxElement->FirstChildElement(NODE_UNIT); max != nullptr;
             max = max->NextSiblingElement(NODE_UNIT))
        {
            // TODO: Insert maximum unit number in buildActionMap
            // int max_number = stoi(max->Attribute(ATTRIBUTE_MAX));
        }

        // Workers
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
                LOG_DEBUG(worker->Attribute(ATTRIBUTE_NAME) << " is a worker!");
            }
        }
        if (!bWorkerFound)
        {
            throw std::out_of_range("No worker was found!");
        }

        action_t gasHarvesterId = -1;
        // Gas harvesters
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
                LOG_DEBUG(gasHarvesterName << " is a gas harvester!");
            }
        }
        if (!bGasHarvesterFound)
        {
            throw std::out_of_range("No worker was found!");
        }

        //get the default supply
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
            LOG_DEBUG("Default Supply for race ["<<currRace.name<<"] is ["<<(*it).second.name<<"]");
            currRace.defaultSupplyAction = (*it).second.id;
        }

        for(auto it = currRace.actions.begin(); it != currRace.actions.end(); ++it)
        {
            if((*it).second.cost.gas > 0)
            {
                LOG_DEBUG("[" << (*it).second.name << "] needs gas!");
                (*it).second.dependencies.push_back(std::pair<action_t, int>(gasHarvesterId, 1));
            }
        }
    }
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
                [&actionName, &actionId](const std::pair<action_t, BuildAction> &entry)
                {
                    //save the actionId here, when search is successfull we need that value
                    actionId = entry.second.id;
                    return entry.second.name.compare(actionName) == 0;
                }
        );

        if (it != race.second.actions.end())
        {
            currentRace = &race.second;
            bFound = true;
            break;
        }
    }
    if (!bFound)
    {

        throw std::out_of_range("Unable to find: " + actionName);
    }
    return currentRace->actions[actionId];
}

const BuildAction &ConfigParser::getAction(action_t id)
{
    if (buildActionIdMap.count(id) == 0)
    {
        throw std::out_of_range("Unable to find: " + std::to_string(id));
    }
    else
    {
        return buildActionIdMap[id];
    }
}

long ConfigParser::getActionCount()
{
    return unitMap.size();
}


action_t ConfigParser::getUnitId(string unitName)
{
    if (unitMap.count(unitName) == 0)
    {
        LOG_DEBUG("Unit added with name " << unitName << " and id " << unitCount);
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

