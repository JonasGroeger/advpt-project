#include <iostream>
#include "ConfigParser.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser configParser(argv[1]);
    BuildAction scvAction = configParser.getAction("scv");
    cout << scvAction.id << " " << scvAction.isWorker << endl;
    BuildAction engineering_bayAction = configParser.getAction("engineering_bay");
    cout << engineering_bayAction.id << " " << engineering_bayAction.isWorker << endl;

    return 0;
}
