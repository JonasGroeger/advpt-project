#include "Game.hpp"
#include "DFBB.hpp"

using namespace std;

int main(int argc, char** argv)
{
    BuildStep::initMap(); // for debugging
    if(argc==2) 
    {
	    Game game(argv[1]);
	    cout << "Hallo Welt";
	    return game.loop();
	}
	else
	{
		cout << argc;
		if(argc!=5)
			return 1;
		DFBB alg;
		// Can cause crash, if argv[4] can't cast to integer
		// Don't bother at the moment
		alg.run(argv[2], argv[3], atoi(argv[4]));
	}
}
