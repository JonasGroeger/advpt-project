#include "ConfigParser.h"

ConfigParser::ConfigParser(char *pathToConfigFile)
{
    configPath = pathToConfigFile;
    XMLError eResult = xmlConfig.LoadFile(configPath);
    if(!checkForXmlError(eResult)){
        //handle error here
    }
    rootNode = xmlConfig.FirstChildElement(NODE_ROOT);
    if(rootNode == nullptr){
        cout << "ERROR" <<endl ;
    }

    //parse the actions
    for (XMLElement* action = rootNode->FirstChildElement(NODE_ACTION); action != nullptr; action = action->NextSiblingElement())
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
            cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
    //TODO where to put them?
    //TODO memory leak here?

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
            cerr << "Out of Range error: " << oor.what() << '\n';
        }
    }
}

BuildAction ConfigParser::getAction(string actionName){
    return buildActionMap.at(actionName);
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

bool ConfigParser::checkForXmlError(XMLError e)
{
    if(e != XML_SUCCESS){
        return false;
    }
    return true;
}
