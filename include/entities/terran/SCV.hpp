#pragma once

#include "Entity.hpp"
#include "entities/Worker.hpp"
#include "Updatable.hpp"
#include "Producer.hpp"

class SCV : public Entity,
            public Worker,
            public Updatable,
            public Producer
{
    /* Should be refactored int common class */
private:
    bool isProducing = false;
    int currentProgress = 0;
    int maxProgress = 0;
    EntityType product = NONE;

    /* class Entity */
public:
    SCV();
    virtual EntityType getType();

    /* class Updatable */
public:
    virtual void update(GameState& state);

    /* class Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState& state);
    virtual void applyChronoBoost();
    virtual long getTimeToFinish();
};
