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
    }
    catch (std::exception &e)
    {
        cerr << "Caught unexpected exception: " << e.what() << endl;
    }

    cout << "SUCCESS" << endl;
    return 0;
}
