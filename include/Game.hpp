#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "BuildOrder.hpp"
#include "BuildStep.hpp"
#include "GameState.hpp"
#include "EntityType.hpp"
#include "entities/protoss/WarpHelper.hpp"

class Game
{
private:
    // This is the only instance of GameState
    // It is owned by Game and only references will be given out
    GameState currentState;
    BuildOrder buildOrder;

    bool executeBuildStep(BuildStep* step);
    bool isAnybodyProducing() const;
    void prepareGame();

public:
    int loop();
    Game(char *file);

    // In case of optimization, we don't start with file but with vector<BuildOrder>
    Game(vector<BuildStep*> buildOrder);
};
