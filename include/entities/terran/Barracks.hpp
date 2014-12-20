#pragma once

#include "Entity.hpp"
#include "Producer.hpp"
#include "Updatable.hpp"

class Barracks: public Entity,
                public Producer,
                public Updatable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 25; // Build time of a marine

    /* class Entity */
    public:
        Barracks();
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