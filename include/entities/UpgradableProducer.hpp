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
    void update(GameState& state);

/* Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) = 0;
    virtual bool upgradeIfPossible(EntityType type, GameState &state) = 0;
    long getTimeToFinish() final;
    void applyChronoBoost() final;
};
