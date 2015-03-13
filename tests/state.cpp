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

    ConfigParser configParser(argv[1]);

    State s = State(configParser);
    s.addUnit(configParser.getUnitId("command_center"));
    s.addUnit(configParser.getUnitId("scv"));

    cout << "Created state with a command_center and one scv" << endl;

    cout << "can build scv: " << s.isLegalAction(configParser.getAction("scv")) << endl;
    cout << "can build supply_depot: " << s.isLegalAction(configParser.getAction("supply_depot")) << endl;
    cout << "can build barracks: " << s.isLegalAction(configParser.getAction("barracks")) << endl;

    cout << "Currently producing " << s.getMineralsPerTick() << " minerals per tick" << endl;
    cout << "Currently producing " << s.getGasPerTick() << " gas per tick" << endl;

    cout << "Barracks can be built in " << s.isAdditionalTimeNeeded(configParser.getAction("barracks")) << endl;

    //s.startAction(configParser.getAction("scv"));
    cout << "SUCCESS" << endl;
    return 0;
}
