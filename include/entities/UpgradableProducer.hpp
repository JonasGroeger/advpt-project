#pragma once

#include "Updatable.hpp"
#include "Upgradable.hpp"
#include "Producer.hpp"

#include "EntityType.hpp"

class Producer;
class Updatable;


/*
 * Every Entity that is simply producing some units should inherit from this class
 * Only produceEntityIfPossible should be overwritten.
 */
class UpgradableProducer : public Producer,
                           public Upgradable,
                           public Updatable
{
/* Updatable */
public:
    virtual void update(GameState& state);
    virtual void printMessageProlog(unsigned int time) const;
    virtual void printBuildEndMessage(EntityType type, unsigned int time) const;

/* Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) = 0;
    virtual bool upgradeIfPossible(EntityType type, GameState &state) = 0;
    bool isProducing() final;
    void applyChronoBoost() final;
};
