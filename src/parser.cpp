#include "parser.h"
using namespace tinyxml2;

Parser::Parser(char *pathToConfigFile)
{
    configPath = pathToConfigFile;
    XMLError eResult = xmlConfig.LoadFile(configPath);
    if(!checkForXmlError(eResult)){
        //handle error here
    }
}

bool Parser::checkForXmlError(XMLError e)
{
    if(e != XML_SUCCESS){
        return false;
    }
    return true;
}
