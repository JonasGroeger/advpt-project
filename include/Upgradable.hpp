#pragma once

#include "EntityType.hpp"

class GameState;

class Upgradable
{
public:
    virtual ~Upgradable()
    {
    }

    virtual bool upgradeIfPossible(EntityType type, GameState &state) = 0;

    virtual bool isUpgrading() = 0;
};
