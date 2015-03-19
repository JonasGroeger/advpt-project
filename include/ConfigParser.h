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
#include <climits>


class ConfigParser
{
public:
    static ConfigParser& Instance()
    {
        static ConfigParser instance;
        return instance;
    }

    void parseConfig(char* file);
    /*
    * This method sets the race used for future calls to getAction(action_t).
    * It does so by looking through all its known actions for @actionName.
    * @param actionName : the action used to determine the race
    * @throws invalid_argument exception when there is no race for @actionName
    */
    void setRaceForAction(string actionName);
    const vector<BuildAction> getAllActions();
    // We return a reference to prevent unneccessary copying
    // But nobody else should be able to modify it
    const map<action_t, int>& getStartConfig();
    const BuildAction& getDefaulSupplyAction();
    const BuildAction& getAction(string actionName);
    const BuildAction& getAction(action_t id);
    unsigned int getActionCount();
    action_t getFirstActionId();

private:
    ConfigParser(){};
    ConfigParser(ConfigParser const&) = delete;
    void operator=(ConfigParser const&) = delete;

    struct Race
    {
        string name;
        action_t defaultSupplyAction;
        map<action_t, int> startUnits;
        map<action_t, BuildAction> actions;
    };

    const char* NODE_RACE = "race";
    const char* NODE_START_UNITS = "start_units";
    const char* NODE_MAX_UNITS = "max_units";
    const char* NODE_WORKER = "workers";
    const char* NODE_GAS_HARVESTER = "gas_harvesters";
    const char* NODE_ACTION = "action";
    const char* NODE_UNIT = "unit";
    const char* NODE_COSTS = "costs";
    const char* NODE_BORROWS = "borrows";
    const char* NODE_DEPENDENCIES = "dependencies";
    const char* NODE_RESULTS = "results";
    const char* NODE_ENERGY = "energy";

    const char* ATTRIBUTE_NAME = "name";
    const char* ATTRIBUTE_COUNT = "count";
    const char* ATTRIBUTE_SPECIAL = "special";
    const char* ATTRIBUTE_HAS_ENERGY = "hasEnergy";
    const char* ATTRIBUTE_ENERGY_FROM = "from";
    const char* ATTRIBUTE_ENERGY_AMOUNT = "amount";
    const char* ATTRIBUTE_ENERGY_START = "start";
    const char* ATTRIBUTE_ENERGY_MAX = "max";
    const char* ATTRIBUTE_DEFAULT_SUPPLY = "default_supply";
    const char* ATTRIBUTE_MINERALS = "mineral";
    const char* ATTRIBUTE_GAS = "gas";
    const char* ATTRIBUTE_TIME = "time";
    const char* ATTRIBUTE_SUPPLY = "supply";
    const char* ATTRIBUTE_MAX = "max";

    XMLDocument xmlConfig;
    map<string, int> unitMap;
    int unitCount = 0;
    map<string, BuildAction> buildActionMap;
    map<action_t, BuildAction> buildActionIdMap;

    int getUnitId(string unitName);
    void addUnitsToVector(XMLElement* element, const char* node, vector<std::pair<action_t, int>>& vec);

    Race currentRace;
    map<string, Race> races;
public:
    const Race getRace();
};
