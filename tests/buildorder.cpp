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

    BuildOrder order("battlecruiser");
    //insert a few unneeded units
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 2));

    //should be possible
    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("barracks").id, 3));

    //list size should now be 0 .. 12 with last entry == battlecruiser

    //should work because 12 == buildList size
    assert(order.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 12));
    //should work
    assert(order.removeActionIfPossible(12));
    //should work
    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("battlecruiser").id, 12));
    //should work
    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("refinery").id, 6));
    //should fail
    assert(!order.replaceActionIfPossible(ConfigParser::Instance().getAction("barracks").id, 10));
    //should work
    assert(order.replaceActionIfPossible(ConfigParser::Instance().getAction("barracks").id, 1));
    //should fail
    assert(!order.removeActionIfPossible(0));
    //should work
    assert(order.removeActionIfPossible(2));

    std::cout << order << endl;

    //check empty one
    BuildOrder empty;
    empty.insertActionIfPossible(ConfigParser::Instance().getAction("supply_depot").id, 0);
    empty.insertActionIfPossible(ConfigParser::Instance().getAction("barracks").id, 1);
    std::cout << empty << endl;

    cout << order.getFitness() << endl;
    cout << "SUCCESS" << endl;
    return 0;
}
