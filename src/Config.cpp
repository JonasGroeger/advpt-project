#include "Config.h"

const char* GENETIC_SECTION = "Genetic";
const char* FIELD_RANDOM_SEED = "RandomSeed";
const char* FIELD_TIME_SEED = "UseTimeSeed";
const char* FIELD_INITIAL_START_LISTS = "InitialRandomStartLists";
const char* FIELD_INITIAL_RANDOM_ENTRIES = "InitialRandomEntries";
const char* FIELD_NUMBER_OF_GENERATIONS = "NumberOfGenerations";
const char* FIELD_RUSH_MAX_TIME = "MaxRushTime";
const char* FIELD_TYPE = "Type";
const char* FIELD_TARGET = "Target";

INIReader reader("config.ini");
#include <iostream>
using namespace std;

// Get a string value from INI file, returning default_value if not found.
std::string getConfigString(std::string section, std::string name,
                std::string default_value)
{
        return reader.Get(section, name, default_value);
}

// Get an integer (long) value from INI file, returning default_value if
// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
long getConfigInteger(std::string section, std::string name, long default_value)
{
        return reader.GetInteger(section, name, default_value);
}

// Get a real (floating point double) value from INI file, returning
// default_value if not found or not a valid floating point value
// according to strtod().
double getConfigDouble(std::string section, std::string name, double default_value)
{
        return reader.GetReal(section, name, default_value);
}

// Get a boolean value from INI file, returning default_value if not found or if
// not a valid true/false value. Valid true values are "true", "yes", "on", "1",
// and valid false values are "false", "no", "off", "0" (not case sensitive).
bool getConfigBoolean(std::string section, std::string name, bool default_value)
{
        return reader.GetBoolean(section, name, default_value);
}
