#include <iostream>
#include <cassert>
#include <vector>

#include "State.h"
#include "ConfigParser.h"

using namespace std; // If you feel offended by this change it!

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

    cerr << "can build scv: " << s.isLegalAction(ConfigParser::Instance().getAction("scv")) << endl;
    cerr << "can build supply_depot: " << s.isLegalAction(ConfigParser::Instance().getAction("supply_depot")) << endl;
    cerr << "can build barracks: " << s.isLegalAction(ConfigParser::Instance().getAction("barracks")) << endl;

    cerr << "Currently producing " << s.getMineralsPerTick() << " minerals per tick" << endl;
    cerr << "Currently producing " << s.getGasPerTick() << " gas per tick" << endl;

    cerr << "Barracks can be built in " << s.isAdditionalTimeNeeded(ConfigParser::Instance().getAction("barracks")) << endl;

    //s.startAction(ConfigParser::Instance().getAction("scv"));
    cerr << "SUCCESS" << endl;
    return 0;
}
