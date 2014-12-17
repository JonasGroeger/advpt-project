#pragma once

#include <algorithm>
#include <iostream>
#include "BuildOrder.hpp"
#include "BuildStep.hpp"
#include "GameState.hpp"
#include "EntityType.hpp"

class Game
{
    // This is the only instance of GameState
    // It is owned by Game and only references will be given out
    GameState currentState;
    BuildOrder buildOrder;

    bool executeBuildStep(BuildStep* step);
public:
    void loop();
    bool isFinished();
    GameState& getFinalState();

    Game(char *file);
};
