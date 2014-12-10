#pragma once

class Game
{
    void executeBuildStep(BuildStep step);
public:
    void loop();
    bool isFinished();
    GameState& getFinalState();
};
