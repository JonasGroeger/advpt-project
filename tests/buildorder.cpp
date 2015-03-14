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
    BuildOrder order("thor");
    std::cout << order << endl;

    std::cout << "INSERTING SUPPLY_DEPOT on position 8" << std::endl;
    b = order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 8);
    std::cout << order << endl;

    std::cout << "INSERTING SUPPLY_DEPOT on position 8" << std::endl;
    b = order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 8);
    std::cout << order << endl;

    std::cout << "TRYING TO REMOVE ELEMENT 2 (factory, should fail!)" << std::endl;
    b = order.removeActionIfPossible(2);
    std::cout << order << endl;

    std::cout << "TRYING TO REMOVE ELEMENT 8 (supply_depot, should work!)" << std::endl;
    b = order.removeActionIfPossible(8);
    std::cout << order << endl;

    std::cout << order << endl;

    cout << "This is the value of b: " << b << endl;
    cout << "I don't know why this variable exists but it must be really important! :)" << endl;
    cout << "SUCCESS" << endl;
    return 0;
}
