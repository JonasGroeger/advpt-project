#pragma once
#include "EntityType.hpp"

class GameState;

class Upgradable
{
public:
    virtual ~Upgradable() = 0;
    virtual bool upgradeIfPossible(EntityType type, GameState &state) = 0;
};
