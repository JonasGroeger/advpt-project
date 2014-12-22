#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"

#include <iostream>

template <EntityType first, EntityType second, int minerals, int gas, int supply, int time>
class UpgradableUnit : public Entity,
                           public Updatable,
                           public Upgradable 
{
private:
    int maxProgress = 0, currentProgress = 0;
    bool morphing = false;

    /* class Entity */
public:
    UpgradableUnit()
    {
        interfaceBitmask = UPDATABLE_INTERFACE | UPGRADABLE_INTERFACE;
        type = first;
    }

    /* class Upgradable */
public:
    virtual bool upgradeIfPossible(EntityType type, GameState &state) override
    {
        // TODO ugly as fuck
        if (!morphing && this->getType() == first && type == second && state.hasEnough(minerals, gas, supply))
        {
            morphing = true;
            state.consumeEnough(minerals, gas, supply);
            state.notifyEntityIsBeingProduced(second);
            currentProgress = 0;
            maxProgress = time;
            return true;
        }
        return false;
    }

    virtual bool isUpgrading() override
    {
        return morphing;
    }

    /* class Updatable */
public:
    virtual void update(GameState &state) override
    {
        if (morphing)
        {
            currentProgress ++;
            
            if (currentProgress > maxProgress)
            {
                state.printBuildEndMessage(second);
                this->setType(second);
                state.setAvailableEntityType(second);
                morphing = false;
                // TODO remove from ugpradables
            }
        }
    }

};
