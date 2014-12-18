#pragma once

#include "GameState.hpp"

class GameState;

class Constructable
{
public:
    virtual bool isConstrucable(GameState& state) {return false;}
    virtual void construct(GameState& state) {}
    virtual ~Constructable() {}
};
