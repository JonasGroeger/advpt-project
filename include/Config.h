#pragma once
#include "INIReader.h"

extern const char* GENETIC_SECTION;
extern const char* FIELD_RANDOM_SEED;
extern const char* FIELD_TIME_SEED;
extern const char* FIELD_INITIAL_START_LISTS;
extern const char* FIELD_INITIAL_RANDOM_ENTRIES;
extern const char* FIELD_NUMBER_OF_GENERATIONS;
extern const char* FIELD_RUSH_MAX_TIME;
extern const char* FIELD_TYPE;
extern const char* FIELD_TARGET;

// Get a string value from INI file, returning default_value if not found.
std::string getConfigString(std::string section, std::string name,
                std::string default_value);

// Get an integer (long) value from INI file, returning default_value if
// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
long getConfigInteger(std::string section, std::string name, long default_value);

// Get a real (floating point double) value from INI file, returning
// default_value if not found or not a valid floating point value
// according to strtod().
double getConfigDouble(std::string section, std::string name, double default_value);

// Get a boolean value from INI file, returning default_value if not found or if
// not a valid true/false value. Valid true values are "true", "yes", "on", "1",
// and valid false values are "false", "no", "off", "0" (not case sensitive).
bool getConfigBoolean(std::string section, std::string name, bool default_value);
