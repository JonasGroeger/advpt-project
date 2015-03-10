#pragma once

#include <tinyxml2.h>

class Parser
{
public:
    Parser(char* pathToConfigFile);

protected:
    bool checkForXmlError(tinyxml2::XMLError e);
    void createAction();

private:
    char* configPath;
    tinyxml2::XMLDocument xmlConfig;
};
