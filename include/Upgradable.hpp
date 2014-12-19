#pragma once
#include "EntityType.hpp"

class GameState;

class Upgradable
{
public:
    virtual ~Upgradable() = 0;
    virtual bool isUpgradable(GameState& state, EntityType type) = 0;
    virtual void upgrade(GameState& state, EntityType to) = 0;
};
