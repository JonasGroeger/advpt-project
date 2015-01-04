#include <iostream>

#include "Game.hpp"
#include "DFBB.hpp"

using namespace std;

int main(int argc, char **argv)
{
    BuildStep::initMap(); // for debugging
    if (argc == 2)
    {
        Game game(argv[1]);
        return game.loop();
    }
    else if (argc == 5) // We need 5 arguments
    {
        // Decide, which algorithm to execute
        string algorithm = argv[1];
        if (algorithm == "DFBB")
        {
            DFBB alg;
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
