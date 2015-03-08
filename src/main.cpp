#include <iostream>
#include <tinyxml2.h>

class Config
{

};

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    std::string configFile = argv[1];

    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError e = doc.LoadFile(configFile.c_str());
    if (e != 0)
    {
        std::cerr << "Could not read configuration file. Is it really there?\n    " << configFile << std::endl;
        return 2;
    }

    //tinyxml2::XMLElement *root = doc.RootElement();

    return 0;
}
