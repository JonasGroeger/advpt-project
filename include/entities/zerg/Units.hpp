#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Producer.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class SpawningPool : public Entity
{
    /* class Entity */
    public:
        SpawningPool(){
            type = ZERG_SPAWNING_POOL;
        }
};

class Zergling : public Entity
{
    /* class Entity */
    public:
        Zergling(){
            type = ZERG_ZERGLING;
        }
};

class Overlord : public Entity
{
    /* class Entity */
    public:
        Overlord(){
            type = ZERG_OVERLORD;
        }
};

// TODO
class Hatchery : public Entity
{
    /* class Entity */
    public:
        Hatchery(){
            type = ZERG_HATCHERY;
        }
};
