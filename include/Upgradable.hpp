#pragma once
#include "EntityType.hpp"

class GameState;

class Upgradable
{
public:
    virtual ~Upgradable();
    virtual bool isUpgradable(GameState& state, EntityType type);
    virtual void upgrade(GameState& state, EntityType to);
};
