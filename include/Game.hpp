#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>
#include "BuildOrder.hpp"
#include "BuildStep.hpp"
#include "GameState.hpp"
#include "EntityType.hpp"

class Game
{
private:
    // This is the only instance of GameState
    // It is owned by Game and only references will be given out
    GameState currentState;
    BuildOrder buildOrder;

    bool executeBuildStep(BuildStep* step);
    bool isAnybodyProducing() const;

    void printMessageProlog() const;
    void printBuildStartMessage(EntityType type) const;
    void printWorkerMessage() const;
    void printResourcesMessage() const;

public:
    int loop();
    bool isFinished();

    void printBuildEndMessage(EntityType type) const;

    GameState& getFinalState();

    Game(char *file);
};
