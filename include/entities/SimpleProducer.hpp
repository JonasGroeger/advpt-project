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
/* Updatable */
public:
    void update(GameState &state) final;

/* Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) = 0;

    bool isProducing() final;

    void applyChronoBoost() final;
};
