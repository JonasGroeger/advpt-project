#pragma once

#include "EntityType.hpp"

// Forward declaration of GameState so this compiles
class GameState;

class Producer
{
public:
    // The virtual functions must have a definition
    // See: http://stackoverflow.com/questions/9406580/c-undefined-reference-to-vtable-and-inheritance
    virtual ~Producer() {}
    virtual bool produceEntityIfPossible(EntityType type, GameState& state) = 0;
    virtual long getTimeToFinish() {return 0;}
    virtual void applyChronoBoost() {}
};
