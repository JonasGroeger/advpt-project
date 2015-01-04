#include "Game.hpp"
#include "DFBB.hpp"

using namespace std;

int main(int argc, char** argv)
{
    BuildStep::initMap(); // for debugging
    if(argc==2) 
    {
        Game game(argv[1]);
        return game.loop();
    }
    else
    {
        // We need 5 arguments
        if(argc!=5)
            return 1;

        // Decide, which algorithm to execute
        string algorithm = argv[1];
        if(algorithm=="DFBB") {
            DFBB alg;
            // Can cause crash, if argv[4] can't cast to integer
            // Don't bother at the moment
            alg.run(argv[2], argv[3], atoi(argv[4]));
        } else {
            // Other algorithms...
        }
    }
}
