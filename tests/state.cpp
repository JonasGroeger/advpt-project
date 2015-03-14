#include <iostream>
#include <cassert>
#include <string>
#include <vector>

#include "State.h"
#include "ConfigParser.h"

using namespace std; // If you feel offended by this change it!

int i = 0;

template <class T>
void simple_test(string msg, T expression, T expected_value)
{
    cerr << "TEST NR." << i++;

    assert(expression == expected_value);
    
    cerr << " " << msg << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);
    ConfigParser::Instance().setRaceForAction("command_center");

    State s = State();
    cerr << "Adding command_center" << endl;
    s.addUnit(ConfigParser::Instance().getAction("command_center").id);
    cerr << "Adding scv" << endl;
    s.addUnit(ConfigParser::Instance().getAction("scv").id);

    cerr << "Created state with a command_center and one scv" << endl;

    simple_test("It is possible to build a scv", s.isLegalAction(ConfigParser::Instance().getAction("scv")), true);
    simple_test("It is possible to build a supply_depot", s.isLegalAction(ConfigParser::Instance().getAction("supply_depot")), true);
    simple_test("It is NOT possible to build a barracks", s.isLegalAction(ConfigParser::Instance().getAction("barracks")), false);
    simple_test("Currently producing 0.7 minerals/second", s.getMineralsPerTick(), (ress_t)70);
    simple_test("Currently producing 0 gas/second", s.getGasPerTick(), (ress_t)0);

    simple_test("supply_depot can be built in 2 ticks", s.isAdditionalTimeNeeded(ConfigParser::Instance().getAction("supply_depot")), (time_t)2);

    cerr << s << endl;

    cerr << "Advancing time by 2" << endl;
    s.advanceTime(2);
    
    cerr << s << endl;

    cerr << "Building supply_depot" << endl;
    s.startAction(ConfigParser::Instance().getAction("supply_depot"));

    cerr << s << endl;

    cerr << "Advancing time by 32" << endl;
    s.advanceTime(32);

    cerr << s << endl;

    cerr << "SUCCESS" << endl;
    return 0;
}
