#include <iostream>
#include <cassert>
#include <vector>

#include "GeneticOptimizer.h"
#include "Config.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);
    ConfigParser::Instance().setRaceForAction("battlecruiser");

    OptimizationStrategy strategy;
    string type = getConfigString(GENETIC_SECTION, FIELD_TYPE, "Rush");
    string target = getConfigString(GENETIC_SECTION, FIELD_TARGET, "marine");

    if (type == "Rush")
    {
        strategy = OptimizationStrategy::Rush;
    }
    else if (type == "Push")
    {
        strategy = OptimizationStrategy::Push;
    }
    else
    {
        cerr << type << endl;
        assert(false);
    }

    cerr << "Starting " << type << " to " << target << endl;
    GeneticOptimizer optimizer(strategy, ConfigParser::Instance().getAction(target).id);
    optimizer.run();
    cout << "SUCCESS" << endl;
}
