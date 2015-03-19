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

    std::cout << "Testing battlecruiser" << std::endl;
    BuildOrder order("battlecruiser");

    //insert a few unneeded units
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));

    assert(order.removeActionIfPossible(2));
    assert(order.removeActionIfPossible(2));
    assert(order.removeActionIfPossible(2));
    assert(order.removeActionIfPossible(2));
    assert(order.removeActionIfPossible(2));


    assert(!order.removeActionIfPossible(0));
    assert(!order.removeActionIfPossible(1));
    assert(!order.removeActionIfPossible(2));
    assert(!order.removeActionIfPossible(3));

    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 1));
    assert(!order.replaceActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 0));
    assert(!order.replaceActionIfPossible(ConfigParser::Instance().getAction("thor").id, 7));
    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("battlecruiser").id, 7));

    std::cout << order << endl;


    std::cout << "Testing protoss colossus" << std::endl;
    ConfigParser::Instance().setRaceForAction("colossus");
    BuildOrder protossOrder("colossus");
    std::cout << protossOrder << std::endl;

    std::cout << "Testing protoss carrier" << std::endl;
    BuildOrder protossCarrier("carrier");
    std::cout << protossCarrier << std::endl;

    cout << "SUCCESS" << endl;
    return 0;
}
