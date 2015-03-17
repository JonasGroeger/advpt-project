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

    GeneticOptimizer optimizer(OptimizationStrategy::Push);

    cout << "SUCCESS" << endl;
}
