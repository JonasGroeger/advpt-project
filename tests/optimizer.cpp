#include <iostream>
#include <cassert>
#include <vector>
#include "GeneticOptimizer.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Please provide a configuration file." << std::endl;
        return 1;
    }

    ConfigParser::Instance().parseConfig(argv[1]);
    ConfigParser::Instance().setRaceForAction("marine");
    //BuildOrder order("brood_lord");
    GeneticOptimizer optimizer(OptimizationStrategy::Rush, ConfigParser::Instance().getAction("marine").id);
    optimizer.run();
    cout << "SUCCESS" << endl;
}
