#pragma once

class Upgradable
{
public:
    virtual bool isUpgradable(GameState state, EntityType type);
    virtual ~Upgradable();
    virtual void upgrade(GameState state, EntityType to);
};
