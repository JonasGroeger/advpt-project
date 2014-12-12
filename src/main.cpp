#include "Game.hpp"

using namespace std;

int main(int argc, char** argv)
{
    Game game(argv[1]);
    game.loop();
    return 0;
}
