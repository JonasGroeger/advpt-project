#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Producer.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class Overlord : public Entity
{
    /* class Entity */
    public:
        Overlord(){
            type = ZERG_OVERLORD;
        }
};

class SpawningPool : public Entity
{
    /* class Entity */
    public:
        SpawningPool(){
            type = ZERG_SPAWNING_POOL;
        }
};

class Roach : public Entity
{
    /* class Entity */
    public:
        Roach(){
            type = ZERG_ROACH;
        }
};

class Hydralisk : public Entity
{
    /* class Entity */
    public:
        Hydralisk(){
            type = ZERG_HYDRALISK;
        }
};

class NydusWorm : public Entity
{
    /* class Entity */
    public:
        NydusWorm(){
            type = ZERG_NYDUS_WORM;
        }
};

class Ultralisk : public Entity
{
    /* class Entity */
    public:
        Ultralisk(){
            type = ZERG_ULTRALISK;
        }
};

class Infestor : public Entity
{
    /* class Entity */
    public:
        Infestor(){
            type = ZERG_INFESTOR;
        }
};

class Corruptor : public Entity
{
    /* class Entity */
    public:
        Corruptor(){
            type = ZERG_CORRUPTOR;
        }
};

class Mutalisk : public Entity
{
    /* class Entity */
    public:
        Mutalisk(){
            type = ZERG_MUTALISK;
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

// TODO
class Hatchery : public Entity
{
    /* class Entity */
    public:
        Hatchery(){
            type = ZERG_HATCHERY;
        }
};

class Extractor : public Entity
{
    /* class Entity */
    public:
        Extractor(){
            type = ZERG_EXTRACTOR;
        }
};

class SpineCrawler : public Entity
{
    /* class Entity */
public:
    SpineCrawler(){
        type = ZERG_SPINE_CRAWLER;
    }
};

class RoachWarren : public Entity
{
    /* class Entity */
public:
    RoachWarren(){
        type = ZERG_ROACH_WARREN;
    }
};

class BanelingNest : public Entity
{
    /* class Entity */
public:
    BanelingNest(){
        type = ZERG_BANELING_NEST;
    }
};

class Spire : public Entity
{
    /* class Entity */
public:
    Spire(){
        type = ZERG_SPIRE;
    }
};

class InfestationPit : public Entity
{
    /* class Entity */
public:
    InfestationPit(){
        type = ZERG_INFESTATION_PIT;
    }
};

class EvolutionChamber : public Entity
{
    /* class Entity */
public:
    EvolutionChamber(){
        type = ZERG_EVOLUTION_CHAMBER;
    }
};

class SporeCrawler : public Entity
{
    /* class Entity */
public:
    SporeCrawler(){
        type = ZERG_SPORE_CRAWLER;
    }
};

class HydraliskDen : public Entity
{
    /* class Entity */
public:
    HydraliskDen(){
        type = ZERG_HYDRALISK_DEN;
    }
};

class NydusNetwork : public Entity
{
    /* class Entity */
public:
    NydusNetwork(){
        type = ZERG_NYDUS_NETWORK;
    }
};

class UltraliskCavern : public Entity
{
    /* class Entity */
public:
    UltraliskCavern(){
        type = ZERG_ULTRALIK_CAVERN;
    }
};
