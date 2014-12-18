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
    virtual bool canProduce(EntityType type, GameState& state) {return false;}
    virtual void produce(EntityType type, GameState& state) {}
    virtual long getTimeToFinish() {return 0;}
    virtual void applyChronoBoost() {}
};
