#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "Producer.hpp"
#include "Constructable.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"

class Updatable;

class SCV : public Entity, 
            public Updatable, 
            public Producer,
            public Constructable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        int maxProgress = 0;
        EntityType product = NONE;

    /* class Entity */
    public:
        virtual EntityType getType()
                ;

    /* class Updatable */
    public:
        virtual void update(GameState& state)
                ;

    /* class Constructable */
    public:
        virtual bool isConstructable(GameState& state)
                ;
        virtual void construct(GameState& state)
                ;
    /* class Producer */
    public:
        virtual bool canProduce(EntityType type, GameState& state)
                ;
        virtual void produce(EntityType type, GameState& state)
                ;
        virtual void applyChronoBoost()
                ;
        virtual long getTimeToFinish();
};

class CommandCenter: public Entity,
                     public Constructable,
                     public Producer,
                     public Updatable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 17; // Build time of scv

    /* class Entity */
    public:
        virtual EntityType getType()
                ;

    /* class Updatable */
    public:
        virtual void update(GameState& state)
                ;

    /* class Constructable */
    public:
        virtual bool isConstructable(GameState& state)
                ;
        virtual void construct(GameState& state)
                ;
    
    /* class Producer */
    public:
        virtual bool canProduce(EntityType type, GameState& state)
                ;
        virtual void produce(EntityType type, GameState& state)
                ;
        virtual void applyChronoBoost()
                ;
        virtual long getTimeToFinish();
};

class SupplyDepot: public Entity,
                   public Constructable
{
    /* class Entity */
    public:
        virtual EntityType getType();

    /* class Constructable */
    public:
        virtual bool isConstructable(GameState& state);
        virtual void construct(GameState& state);
};

class Barracks: public Entity,
                public Constructable,
                public Producer,
                public Updatable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 25; // Build time of scv

    /* class Entity */
    public:
        virtual EntityType getType();

    /* class Updatable */
    public:
        virtual void update(GameState& state);

    /* class Constructable */
    public:
        virtual bool isConstructable(GameState& state);
        virtual void construct(GameState& state);
    
    /* class Producer */
    public:
        virtual bool canProduce(EntityType type, GameState& state);
        virtual void produce(EntityType type, GameState& state);
        virtual void applyChronoBoost();
        virtual long getTimeToFinish();
};

class Marine: public Entity
{
    /* class Entity */
    public:
        virtual EntityType getType();
};
