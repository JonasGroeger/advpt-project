#include <iostream>
#include "parser.h"

class Config
{

};

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    Parser configParser(argv[1]);

    return 0;
}
