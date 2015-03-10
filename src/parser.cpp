#include "parser.h"

Parser::Parser(char *pathToConfigFile)
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

    //first parse the workers
    XMLElement *workerElement = rootNode->FirstChildElement(NODE_WORKER);
    for (XMLElement* worker = workerElement->FirstChildElement(); worker != nullptr; worker = worker->NextSiblingElement())
    {
        addUnitToMap(worker->Attribute(ATTRIBUTE_NAME));
    }
    //TODO where to put them?
    //TODO memory leak here?

    //now parse the gas_harvesters
    XMLElement *gas_harvesters = rootNode->FirstChildElement(NODE_GAS_HARVESTER);
    for (XMLElement* gas_element = gas_harvesters->FirstChildElement(); gas_element != nullptr; gas_element = gas_element->NextSiblingElement())
    {
        addUnitToMap(gas_element->Attribute(ATTRIBUTE_NAME));
    }

    //now the rest
    XMLElement *actions = rootNode->FirstChildElement(NODE_ACTION);
    for (XMLElement* action = actions->FirstChildElement(); action != nullptr; action = action->NextSiblingElement())
    {
        //TODO
    }
}

void Parser::addUnitToMap(string unitName)
{
    if(unitMap.count(unitName) == 0)
    {
        cout << "Unit added with name " << unitName << " and id " << unitCount << endl;
        unitMap.insert(std::pair<string, int>(unitName, unitCount));
        ++unitCount;
    }
}

bool Parser::checkForXmlError(XMLError e)
{
    if(e != XML_SUCCESS){
        return false;
    }
    return true;
}
