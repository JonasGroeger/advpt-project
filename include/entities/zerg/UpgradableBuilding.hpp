#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"

template <EntityType first, EntityType second, int minerals, int gas, int time>
class UpgradableBuilding : public Entity,
                           public Updatable,
                           public Upgradable 
{
    /* class Entity */
public:
    UpgradableBuilding()
    {
        interfaceBitmask = UPDATABLE_INTERFACE | UPGRADABLE_INTERFACE;
        type = first;
    }

    /* class Upgradable */
public:
    virtual bool upgradeIfPossible(EntityType type, GameState &state) override
    {
        if (type == second)
        {
            this->setType(second);
            state.setAvailableEntityType(second);
            //state.printBuildEndMessage(second);
            return true;
        }
        return false;
    }

    /* class Updatable */
public:
    virtual void update(GameState &state) override
    {
    }

};
