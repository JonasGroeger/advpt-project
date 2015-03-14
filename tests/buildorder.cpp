#include <iostream>
#include <cassert>
#include <vector>
#include <BuildOrder.h>

#include "ConfigParser.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);

    bool b = false;
    BuildOrder order("battlecruiser");

    std::cout << order << endl;

    cout << "This is the value of b: " << b << endl;
    cout << "I don't know why this variable exists but it must be really important! :)" << endl;
    cout << "SUCCESS" << endl;
    return 0;
}
