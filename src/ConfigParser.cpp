#include "ConfigParser.h"

ConfigParser::ConfigParser(char *pathToConfigFile)
{
    cout << pathToConfigFile << endl;
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
        for (XMLElement* costUnits = costs->FirstChildElement(); costUnits != nullptr; costUnits = costUnits->NextSiblingElement())
        {
            addUnitToVector(costUnits->Attribute(ATTRIBUTE_NAME), buildAction.cost.units);
        }

        //borrows
        XMLElement* borrows = action->FirstChildElement(NODE_BORROWS);
        for (XMLElement* borrow = borrows->FirstChildElement(); borrow != nullptr; borrow = borrow->NextSiblingElement())
        {
            addUnitToVector(borrow->Attribute(ATTRIBUTE_NAME), buildAction.borrows);
        }

        //dependencies
        XMLElement* dependencies = action->FirstChildElement(NODE_DEPENDENCIES);
        for (XMLElement* dep = dependencies->FirstChildElement(); dep != nullptr; dep = dep->NextSiblingElement())
        {
            addUnitToVector(dep->Attribute(ATTRIBUTE_NAME), buildAction.dependencies);
        }

        //results
        XMLElement* results = action->FirstChildElement(NODE_RESULTS);

        buildAction.result.minerals = stoi(results->Attribute(ATTRIBUTE_MINERALS));
        buildAction.result.gas = stoi(results->Attribute(ATTRIBUTE_GAS));
        buildAction.result.supply = stoi(results->Attribute(ATTRIBUTE_SUPPLY));
        for (XMLElement* resultUnit = results->FirstChildElement(NODE_UNIT); resultUnit != nullptr; resultUnit = resultUnit->NextSiblingElement())
        {
            addUnitToVector(resultUnit->Attribute(ATTRIBUTE_NAME), buildAction.result.units);
        }
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

void ConfigParser::addUnitToVector(string unitName, vector<int>& vec){
    getUnitId(unitName);
    vec.push_back(unitMap[unitName]);
}

bool ConfigParser::checkForXmlError(XMLError e)
{
    if(e != XML_SUCCESS){
        return false;
    }
    return true;
}
