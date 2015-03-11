#include "ConfigParser.h"

ConfigParser::ConfigParser(char *file)
{
    XMLError load_result = xmlConfig.LoadFile(file);
    if(load_result != XML_SUCCESS){
        throw std::invalid_argument("Malformed configuration file.");
    }

    XMLNode *rootNode = xmlConfig.RootElement();
    if(rootNode == nullptr){
        throw std::invalid_argument("Malformed configuration file: No root element found.");
    }

    // parse the actions
    for (XMLElement* action = rootNode->FirstChildElement(NODE_ACTION); action != nullptr;
         action = action->NextSiblingElement("action"))
    {
        BuildAction buildAction;
        buildAction.name = action->Attribute(ATTRIBUTE_NAME);
        buildAction.id = getUnitId(buildAction.name);

        //costs of the action
        XMLElement* costs = action->FirstChildElement(NODE_COSTS);
        buildAction.cost.gas = stoi(costs->Attribute(ATTRIBUTE_GAS));
        buildAction.cost.minerals = stoi(costs->Attribute(ATTRIBUTE_MINERALS));
        buildAction.cost.supply = stoi(costs->Attribute(ATTRIBUTE_SUPPLY));
        buildAction.cost.time = stoi(costs->Attribute(ATTRIBUTE_TIME));
        //if this action is consuming other units
        addUnitsToVector(costs, NODE_UNIT, buildAction.cost.units);

        //borrows
        XMLElement* borrows = action->FirstChildElement(NODE_BORROWS);
        addUnitsToVector(borrows, NODE_UNIT, buildAction.borrows);

        //dependencies
        XMLElement* dependencies = action->FirstChildElement(NODE_DEPENDENCIES);
        addUnitsToVector(dependencies, NODE_UNIT, buildAction.dependencies);

        //results
        XMLElement* results = action->FirstChildElement(NODE_RESULTS);

        buildAction.result.minerals = stoi(results->Attribute(ATTRIBUTE_MINERALS));
        buildAction.result.gas = stoi(results->Attribute(ATTRIBUTE_GAS));
        buildAction.result.supply = stoi(results->Attribute(ATTRIBUTE_SUPPLY));
        addUnitsToVector(results, NODE_UNIT, buildAction.result.units);

        buildActionMap[buildAction.name] = buildAction;
    }

    //workers
    XMLElement *workerElement = rootNode->FirstChildElement(NODE_WORKER);
    for (XMLElement* worker = workerElement->FirstChildElement(); worker != nullptr; worker = worker->NextSiblingElement())
    {
        try
        {
            buildActionMap.at(worker->Attribute(ATTRIBUTE_NAME)).isWorker = true;
            cout << worker->Attribute(ATTRIBUTE_NAME) << " is a worker!" << endl;
        }
        catch (const std::out_of_range& oor)
        {
            throw std::out_of_range(worker->Attribute(ATTRIBUTE_NAME) + string(" is not present in the map."));
        }
    }

    //now parse the gas_harvesters
    XMLElement *gas_harvesters = rootNode->FirstChildElement(NODE_GAS_HARVESTER);
    for (XMLElement* gas_element = gas_harvesters->FirstChildElement(); gas_element != nullptr; gas_element = gas_element->NextSiblingElement())
    {

        try
        {
            buildActionMap.at(gas_element->Attribute(ATTRIBUTE_NAME)).isGasHarvester = true;
            cout << gas_element->Attribute(ATTRIBUTE_NAME) << " is a gas harvester!" << endl;
        }
        catch (const std::out_of_range& oor)
        {
            throw std::out_of_range(gas_element->Attribute(ATTRIBUTE_NAME) + string(" is not present in the map."));
        }
    }
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


int ConfigParser::getUnitId(string unitName)
{
    if(unitMap.count(unitName) == 0)
    {
        cout << "Unit added with name " << unitName << " and id " << unitCount << endl;
        unitMap.insert(std::pair<string, int>(unitName, unitCount));
        ++unitCount;
    }
    return unitMap[unitName];
}

void ConfigParser::addUnitsToVector(XMLElement* element, const char* node, vector<int>& targetVector){
    for (XMLElement* tmpElement = element->FirstChildElement(node); tmpElement != nullptr; tmpElement = tmpElement->NextSiblingElement())
    {
        string unitName = tmpElement->Attribute(ATTRIBUTE_NAME);
        getUnitId(unitName);
        targetVector.push_back(unitMap[unitName]);
    }
}

