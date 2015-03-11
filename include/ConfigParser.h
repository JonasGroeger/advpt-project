#pragma once

#include <tinyxml2.h>
#include <iostream>
#include <string>
#include <map>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "Debug.h"
#include "BuildAction.h"

using tinyxml2::XML_SUCCESS;
using tinyxml2::XMLDocument;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XMLNode;

using std::string;
using std::map;
using std::stoi;
using std::vector;

class ConfigParser
{
public:
    ConfigParser(char*file);

    // We return a reference to prevent unneccessary copying
    // But nobody else should be able to modify it
    const BuildAction& getAction(string actionName);

protected:
    int getUnitId(string unitName);
    void addUnitsToVector(XMLElement* element, const char* node, vector<std::pair<action_t, int>>& vec);

private:
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
    const char* ATTRIBUTE_MINERALS = "mineral";
    const char* ATTRIBUTE_GAS = "gas";
    const char* ATTRIBUTE_TIME = "time";
    const char* ATTRIBUTE_SUPPLY = "supply";

    XMLDocument xmlConfig;
    map<string, int> unitMap;
    int unitCount = 0;
    map<string, BuildAction> buildActionMap;
};
