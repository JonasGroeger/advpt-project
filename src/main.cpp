#include <iostream>
#include "ConfigParser.h"
#include "BuildOrder.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);
    BuildOrder buildOrder;
    buildOrder.createBuildOrder("thor");
    return 0;
}
