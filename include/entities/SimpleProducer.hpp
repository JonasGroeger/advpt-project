#pragma once

#include "Updatable.hpp"
#include "Producer.hpp"

#include "EntityType.hpp"

class Producer;
class Updatable;

/*
 * Every Entity that is simply producing some units should inherit from this class
 * Only produceEntityIfPossible should be overwritten.
 */
class SimpleProducer : public Producer,
                       public Updatable
{
protected:
    bool isProducing = false;
    int currentProgress = 0;
    int maxProgress = 0;
    EntityType product = NONE;

/* Updatable */
public:
    void update(GameState& state) final;

/* Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) = 0;
    long getTimeToFinish() final;
    void applyChronoBoost() final;
};
