#pragma once

#include "GameState.hpp"

// Forward declaration of GameState so this compiles
class GameState;

class Upgradable
{
public:
    virtual bool isUpgradable(GameState& state, EntityType type);
    virtual ~Upgradable();
    virtual void upgrade(GameState& state, EntityType to);
};
