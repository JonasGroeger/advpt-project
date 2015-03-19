#include <iostream>
#include <cassert>
#include <vector>

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
    ConfigParser::Instance().setRaceForAction("scv");
    bool b = false;
    try 
    {
        BuildAction asdf = ConfigParser::Instance().getAction("asdf");
    }
    catch (std::out_of_range &oof)
    {
        b = true;
    }
    if (!b)
    {
        cerr << "config.Parser.getAction(\"asdf\"); did not throw an out_of_range exception" << endl;
        return 1;
    }
    cout << 1 << endl;

    try
    {
        BuildAction scv = ConfigParser::Instance().getAction("scv");
        BuildAction tank = ConfigParser::Instance().getAction("siege_tank");
        BuildAction refinery = ConfigParser::Instance().getAction("refinery");
        BuildAction commandCenter = ConfigParser::Instance().getAction("command_center");

        assert(scv.isWorker);
        cout << 2 << endl;
        assert(!commandCenter.isWorker);
        cout << 3 << endl;
        assert(!refinery.isWorker);
        cout << 4 << endl;

        assert(commandCenter.dependencies.size() == 0);
        cout << 5 << endl;
        assert(commandCenter.borrows.size() == 1);
        assert(commandCenter.borrows[0].first == scv.id);
        assert(commandCenter.borrows[0].second == 1);

        BuildAction orbitalCommand = ConfigParser::Instance().getAction("orbital_command");
        assert(orbitalCommand.hasEnergy);
        assert(orbitalCommand.startEnergy == 50);
        assert(orbitalCommand.maxEnergy = 200);
        assert(orbitalCommand.isSpecial == false);
        assert(orbitalCommand.cost.energyAmount == 0);

        BuildAction mule = ConfigParser::Instance().getAction("mule");
        assert(mule.hasEnergy == false);
        assert(mule.startEnergy == 0);
        assert(mule.maxEnergy == 0);
        assert(mule.isSpecial == true);
        assert(mule.cost.energyAmount == 50);
        assert(mule.cost.energyFrom == ConfigParser::Instance().getAction("orbital_command").id);

        //protoss
        ConfigParser::Instance().setRaceForAction("probe");

        BuildAction chronoboost = ConfigParser::Instance().getAction("chrono_boost");
        assert(chronoboost.hasEnergy == false);
        assert(chronoboost.startEnergy == 0);
        assert(chronoboost.maxEnergy == 0);
        assert(chronoboost.isSpecial == true);
        assert(chronoboost.cost.energyAmount == 25);
        assert(chronoboost.cost.energyFrom == ConfigParser::Instance().getAction("nexus").id);

        BuildAction nexus = ConfigParser::Instance().getAction("nexus");
        assert(nexus.hasEnergy == true);
        assert(nexus.startEnergy == 0);
        assert(nexus.maxEnergy == 100);
        assert(nexus.isSpecial == false);
        assert(nexus.cost.energyAmount == 0);

        //zerg
        ConfigParser::Instance().setRaceForAction("queen");

        BuildAction queen = ConfigParser::Instance().getAction("queen");
        assert(queen.hasEnergy == true);
        assert(queen.startEnergy == 50);
        assert(queen.maxEnergy == 200);
        assert(queen.isSpecial == false);
        assert(queen.cost.energyAmount == 0);

        BuildAction spawn_larva = ConfigParser::Instance().getAction("spawn_larva");
        assert(spawn_larva.hasEnergy == false);
        assert(spawn_larva.startEnergy == 0);
        assert(spawn_larva.maxEnergy == 0);
        assert(spawn_larva.isSpecial == true);
        assert(spawn_larva.cost.energyAmount == 25);
        assert(spawn_larva.cost.energyFrom == ConfigParser::Instance().getAction("queen").id);

    }
    catch (std::exception &e)
    {
        cerr << "Caught unexpected exception: " << e.what() << endl;
    }

    cout << "SUCCESS" << endl;
    return 0;
}
