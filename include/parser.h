#pragma once

#include <tinyxml2.h>
using namespace tinyxml2;

class Parser
{
public:
    Parser(char* pathToConfigFile);

protected:
    bool checkForXmlError(tinyxml2::XMLError e);
    void createAction();

private:
    char* configPath;
    XMLDocument xmlConfig;
};
