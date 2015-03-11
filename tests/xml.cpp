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

    ConfigParser configParser(argv[1]);

    bool b = false;
    try 
    {
        BuildAction asdf = configParser.getAction("asdf");
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

    try
    {
        BuildAction scv = configParser.getAction("scv");
        BuildAction tank = configParser.getAction("siege_tank");
        BuildAction refinery = configParser.getAction("refinery");
        BuildAction commandCenter = configParser.getAction("command_center");

        assert(scv.isWorker);
        assert(!commandCenter.isWorker);
        assert(!refinery.isWorker);

        assert(commandCenter.dependencies.size() == 0);
        assert(commandCenter.borrows == vector<int>{scv.id});
    }
    catch (std::exception &e)
    {
        cerr << "Caught unexpected exception: " << e.what() << endl;
    }

    return 0;
}
