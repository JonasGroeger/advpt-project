#pragma once

#include "Updatable.hpp"
#include "Upgradable.hpp"
#include "Producer.hpp"

#include "EntityType.hpp"

class Producer;
class Updatable;

enum class UPState {IDLE, PRODUCING, UPGRADING};

/*
 * Every Entity that is simply producing some units should inherit from this class
 * Only produceEntityIfPossible should be overwritten.
 */
class UpgradableProducer : public Producer,
                           public Upgradable,
                           public Updatable
{
protected:
    UPState state = UPState::IDLE;
    int currentProgress = 0;
    int maxProgress = 0;
    EntityType product = NONE;
    bool isBusy();

/* Updatable */
public:
    void update(GameState& state) final;

/* Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) = 0;
    virtual bool upgradeIfPossible(EntityType type, GameState &state) = 0;
    long getTimeToFinish() final;
    void applyChronoBoost() final;
};
