#include <iostream>

#include "Game.hpp"
#include "DepthFirstBranchAndBound.hpp"

using namespace std;

int main(int argc, char **argv)
{
    BuildStep::initMap(); // for debugging
    if (argc == 2)
    {
        BuildOrder order = BuildOrder(argv[1]);
        unsigned int time = Game::getFitnessPush(order);
        printf("Completed list in %d seconds\n", time);
        return 0;
    }
    else if (argc == 5) // We need 5 arguments
    {
        // Decide, which algorithm to execute
        string algorithm = argv[1];
        if (algorithm == "DFBB")
        {
            DepthFirstBranchAndBound alg;
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
