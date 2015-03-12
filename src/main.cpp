#include <iostream>
#include "ConfigParser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);

    return 0;
}
