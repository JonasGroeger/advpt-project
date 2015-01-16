#include <iostream>

#include "Game.hpp"
#include "GeneticOptimizer.hpp"
#include "DepthFirstBranchAndBound.hpp"
#include "Config.hpp"

using namespace std;

int main(int argc, char **argv)
{
    cout << getConfigBoolean("Test", "test_val1", false) << endl;
    cout << getConfigBoolean("Test", "test_val2", false) << endl;
    cout << getConfigBoolean("Test", "test_val3", false) << endl;
    cout << getConfigInteger("Test", "test_val4", false) << endl;
    cout << getConfigString("Test", "test_val5", "default") << endl;
    BuildStep::initMap(); // for debugging
    if (argc == 2)
    {
        BuildOrder order(argv[1]);
        if (!order.isPossible())
        {
            cerr << "This build order is invalid" << endl;
            return 1;
        }
        unsigned int time = Game::getFitnessPush(order);
        printf("Completed list in %d seconds\n", time);
        return 0;
    }
    else if (argc == 5) // We need 5 arguments
    {
        
        // Decide, which algorithm to execute
        string algorithm = argv[1];
        if (algorithm == "GENETIC")
        {
            GeneticOptimizer alg;
            // Will throw invalid_argument exception if argv[4] can't be converted
            alg.run(argv[2], argv[3], stoi(argv[4]));
        }
        else
        {
            // Other algorithms...
        }
    }
    else
    {
        std::cerr << "Invalid number of arguments! " << std::endl;
        std::cerr << "USAGE:" << std::endl;
        std::cerr << '\t' << argv[0] << " <build-order-file>" << std::endl;
        std::cerr << '\t' << argv[0] << " DFBB <entity> <mode> <max-simulation-time>" << std::endl;
    }
}
