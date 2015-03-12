#include "ConfigParser.h"

void ConfigParser::parseConfig(char *file){
    XMLError load_result = xmlConfig.LoadFile(file);
    if(load_result != XML_SUCCESS){
        throw std::invalid_argument("Malformed configuration file.");
    }

    XMLNode *rootNode = xmlConfig.RootElement();
    if(rootNode == nullptr){
        throw std::invalid_argument("Malformed configuration file: No root element found.");
    }

    // Iterate through found races
    for(XMLElement *race = rootNode->FirstChildElement(NODE_RACE); race != nullptr;
        race = race->NextSiblingElement(NODE_RACE))
    {

        // parse the actions
        for (XMLElement *action = race->FirstChildElement(NODE_ACTION); action != nullptr;
             action = action->NextSiblingElement(NODE_ACTION))
        {
            BuildAction buildAction;
            buildAction.name = action->Attribute(ATTRIBUTE_NAME);
            buildAction.id = getUnitId(buildAction.name);

            //costs of the action
            XMLElement *costs = action->FirstChildElement(NODE_COSTS);
            buildAction.cost.gas = costs->IntAttribute(ATTRIBUTE_GAS);
            buildAction.cost.minerals = costs->IntAttribute(ATTRIBUTE_MINERALS);
            buildAction.cost.supply = costs->IntAttribute(ATTRIBUTE_SUPPLY);
            buildAction.cost.time = costs->IntAttribute(ATTRIBUTE_TIME);
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

            buildAction.result.minerals = results->IntAttribute(ATTRIBUTE_MINERALS);
            buildAction.result.gas = results->IntAttribute(ATTRIBUTE_GAS);
            buildAction.result.supply = results->IntAttribute(ATTRIBUTE_SUPPLY);
            addUnitsToVector(results, NODE_UNIT, buildAction.result.units);

            buildActionMap[buildAction.name] = buildAction;
        }

    }

    // Maximum unit numbers
    XMLElement *maxElement = rootNode->FirstChildElement(NODE_MAX_UNITS);
    for(XMLElement *max = maxElement->FirstChildElement(NODE_UNIT); max != nullptr;
            max = max->NextSiblingElement(NODE_UNIT))
    {
        // TODO: Insert maximum unit number in buildActionMap
        // int max_number = stoi(max->Attribute(ATTRIBUTE_MAX));
    }

    // Workers
    XMLElement *workerElement = rootNode->FirstChildElement(NODE_WORKER);
    for (XMLElement* worker = workerElement->FirstChildElement(); worker != nullptr;
         worker = worker->NextSiblingElement(NODE_UNIT))
    {
        try
        {
            buildActionMap.at(worker->Attribute(ATTRIBUTE_NAME)).isWorker = true;
            LOG_DEBUG(worker->Attribute(ATTRIBUTE_NAME) << " is a worker!");
        }
        catch (const std::out_of_range& oor)
        {
            throw std::out_of_range(worker->Attribute(ATTRIBUTE_NAME) + string(" is not present in the map."));
        }
    }

    // Gas harvesters
    XMLElement *gas_harvesters = rootNode->FirstChildElement(NODE_GAS_HARVESTER);
    for (XMLElement* gas_element = gas_harvesters->FirstChildElement(); gas_element != nullptr;
         gas_element = gas_element->NextSiblingElement(NODE_UNIT))
    {
        try
        {
            buildActionMap.at(gas_element->Attribute(ATTRIBUTE_NAME)).isGasHarvester = true;
            LOG_DEBUG(gas_element->Attribute(ATTRIBUTE_NAME) << " is a gas harvester!");
        }
        catch (const std::out_of_range& oor)
        {
            throw std::out_of_range(gas_element->Attribute(ATTRIBUTE_NAME) + string(" is not present in the map."));
        }
    }
}

const vector<BuildAction> ConfigParser::getAllActions()
{
    vector<BuildAction> resultVec;
    for(auto pair : buildActionMap)
    {
        resultVec.push_back(pair.second);
    }
    return resultVec;
}

const BuildAction& ConfigParser::getAction(string actionName)
{
    if (buildActionMap.count(actionName) == 0)
    {
        throw std::out_of_range("Unable to find: " + actionName);
    }
    else
    {
        return buildActionMap[actionName];
    }
}

long ConfigParser::getActionCount()
{
    return unitMap.size();
}


action_t ConfigParser::getUnitId(string unitName)
{
    if(unitMap.count(unitName) == 0)
    {
        LOG_DEBUG( "Unit added with name " << unitName << " and id " << unitCount);
        unitMap.insert(std::pair<string, int>(unitName, unitCount));
        ++unitCount;
    }
    return unitMap[unitName];
}

void ConfigParser::addUnitsToVector(XMLElement* element, const char* node, vector<std::pair<action_t, int>>& targetVector){
    for (XMLElement* tmpElement = element->FirstChildElement(node); tmpElement != nullptr; tmpElement = tmpElement->NextSiblingElement())
    {
        string unitName = tmpElement->Attribute(ATTRIBUTE_NAME);
        action_t id = getUnitId(unitName);

        std::vector<std::pair<action_t, int>>::iterator it;
        it = std::find_if(targetVector.begin(), targetVector.end(), [&id](std::pair<action_t, int> &pair)
        {
            return (pair.first == id);
        });

        if(it != targetVector.end())
        {
            (*it).second++;
        }
        else
        {
            targetVector.push_back(std::pair<action_t, int>(unitMap[unitName], 1));
        }
    }
}

