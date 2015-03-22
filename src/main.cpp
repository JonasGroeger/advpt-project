#include <iostream>
#include <GeneticOptimizer.h>
#include <AddressBook/AddressBook.h>
#include "ConfigParser.h"
#include "BuildOrder.h"
#include "Config.h"

#include <fstream>
#include <string>
#include <algorithm>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }
    ConfigParser::Instance().parseConfig(argv[1]);

    //Optimizer
    if (argc == 2)
    {
        std::cout << "Starting optimizer" << std::endl;

        string target = getConfigString(GENETIC_SECTION, FIELD_TARGET, "marine");
        ConfigParser::Instance().setRaceForAction(target);

        string type = getConfigString(GENETIC_SECTION, FIELD_TYPE, "push");
        OptimizationStrategy strategy;

        if(type == "rush")
        {
            strategy = OptimizationStrategy::Rush;
        }
        if(type == "push")
        {
            strategy = OptimizationStrategy::Push;
        }

        GeneticOptimizer optimizer(strategy, ConfigParser::Instance().getAction(target).id);
        optimizer.run();
    }
    //forwardSim
    if(argc == 3)
    {
        std::cout << "Starting forwardSim" << std::endl;
        vector<string> buildListVec;
        char* buildList = argv[2];
        ifstream is(buildList);
        string str;
        while(getline(is, str))
        {
            buildListVec.push_back(str);
        }
        ConfigParser::Instance().setRaceForAction(buildListVec[0]);
        BuildOrder mBuildOrder(buildListVec);
    }



    ConfigParser::Instance().parseConfig(argv[1]);
    return 0;
}
