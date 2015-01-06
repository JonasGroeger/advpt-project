#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Producer.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class SCV : public Entity,
            public Worker,
            public Updatable,
            public Producer
{
    /* Should be refactored int common class */
private:
    int currentProgress = 0;
    int maxProgress = 0;
    EntityType product = NONE;

    /* class Entity */
public:
    SCV();

    /* class Updatable */
public:
    virtual void update(GameState &state);

    /* class Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state);

    virtual void applyChronoBoost();

    virtual bool isProducing();
};

class PlanetaryFortress : public Entity,
                          public Producer
{
public:
    PlanetaryFortress()
    {
        interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
        type = TERRAN_PLANETARY_FORTRESS;
    }

    /* class Producer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state);

    virtual bool isProducing() override;

    virtual void applyChronoBoost() override;

};