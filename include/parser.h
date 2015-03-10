#pragma once

#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <map>
using namespace tinyxml2;
using std::cout;
using std::endl;
using std::string;
using std::map;

class Parser
{
public:
    Parser(char* pathToConfigFile);

protected:
    bool checkForXmlError(tinyxml2::XMLError e);
    void parseWorkers();
    void parseGasHarvesters();
    void createAction();

    void addUnitToMap(string unitName);

private:
    const char* NODE_ROOT = "sc2sim";
    const char* NODE_MAX_UNITS = "max_units";
    const char* NODE_WORKER = "workers";
    const char* NODE_GAS_HARVESTER = "gas_harvesters";
    const char* NODE_ACTION = "action";
    const char* NODE_UNIT = "unit";
    const char* NODE_COSTS = "costs";
    const char* NODE_BORROWS = "borrows";
    const char* NODE_DEPENDENCIES = "dependencies";
    const char* NODE_RESULTS = "results";

    const char* ATTRIBUTE_NAME = "name";
    const char* ATTRIBUTE_MINERALS = "minerals";
    const char* ATTRIBUTE_GAS = "gas";
    const char* ATTRIBUTE_TIME = "time";
    const char* ATTRIBUTE_SUPPLY = "supply";

    char* configPath;
    XMLDocument xmlConfig;
    XMLNode* rootNode;
    map<string, int> unitMap;
    int unitCount = 0;
};
