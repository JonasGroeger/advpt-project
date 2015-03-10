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

    //first parse the workers
    XMLElement *workerElement = rootNode->FirstChildElement(NODE_WORKER);
    for (XMLElement* worker = workerElement->FirstChildElement(); worker != nullptr; worker = worker->NextSiblingElement())
    {
        getUnitId(worker->Attribute(ATTRIBUTE_NAME));
    }
    //TODO where to put them?
    //TODO memory leak here?

    //now parse the gas_harvesters
    XMLElement *gas_harvesters = rootNode->FirstChildElement(NODE_GAS_HARVESTER);
    for (XMLElement* gas_element = gas_harvesters->FirstChildElement(); gas_element != nullptr; gas_element = gas_element->NextSiblingElement())
    {
        getUnitId(gas_element->Attribute(ATTRIBUTE_NAME));
    }

    //now the rest
    for (XMLElement* action = rootNode->FirstChildElement(NODE_ACTION); action != nullptr; action = action->NextSiblingElement())
    {
        BuildAction buildAction;
        buildAction.name = action->Attribute(ATTRIBUTE_NAME);

        //costs of the action
        XMLElement* costs = action->FirstChildElement(NODE_COSTS);
        buildAction.neededGas = stoi(costs->Attribute(ATTRIBUTE_GAS));
        buildAction.neededMinerals = stoi(costs->Attribute(ATTRIBUTE_MINERALS));
        buildAction.neededSupply = stoi(costs->Attribute(ATTRIBUTE_SUPPLY));
        buildAction.neededTime = stoi(costs->Attribute(ATTRIBUTE_TIME));


        //borrows
        XMLElement* borrows = action->FirstChildElement(NODE_BORROWS);
        for (XMLElement* borrow = borrows->FirstChildElement(); borrow != nullptr; borrow = borrow->NextSiblingElement())
        {
            string borrowedUnit = borrow->Attribute(ATTRIBUTE_NAME);
            getUnitId(borrowedUnit);
            buildAction.borrows.push_back(unitMap[borrowedUnit]);
        }

        //dependencies
        XMLElement* dependencies = action->FirstChildElement(NODE_DEPENDENCIES);
        for (XMLElement* dep = dependencies->FirstChildElement(); dep != nullptr; dep = dep->NextSiblingElement())
        {
            string depUnit = dep->Attribute(ATTRIBUTE_NAME);
            getUnitId(depUnit);
            buildAction.dependencies.push_back(unitMap[depUnit]);
        }

        //results
        XMLElement* results = action->FirstChildElement(NODE_RESULTS);

        buildAction.result.minerals = stoi(results->Attribute(ATTRIBUTE_MINERALS));
        buildAction.result.gas = stoi(results->Attribute(ATTRIBUTE_GAS));
        buildAction.result.supply = stoi(results->Attribute(ATTRIBUTE_SUPPLY));
        for (XMLElement* resultUnit = results->FirstChildElement(NODE_UNIT); resultUnit != nullptr; resultUnit = resultUnit->NextSiblingElement())
        {
            string res = resultUnit->Attribute(ATTRIBUTE_NAME);
            getUnitId(res);
            buildAction.result.units.push_back(unitMap[res]);
        }
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

bool ConfigParser::checkForXmlError(XMLError e)
{
    if(e != XML_SUCCESS){
        return false;
    }
    return true;
}
