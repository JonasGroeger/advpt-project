#pragma once

#include "Entity.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class Probe : public Entity,
              public Worker,
              public Updatable
{
    /* class Entity */
    public:
        Probe();

    /* class Updatable */
    public:
        void update(GameState &state);
};
