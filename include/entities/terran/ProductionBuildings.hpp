#pragma once

#include "Entity.hpp"
//#include "Upgradable.hpp"
#include "entities/SimpleProducer.hpp"

class CommandCenter: public Entity,
                     public SimpleProducer
{
    /* class Entity */
    public:
        CommandCenter();
        virtual EntityType getType() override;

    /* class SimpleProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
};

class Barracks: public Entity,
                public SimpleProducer
{
    /* class Entity */
    public:
        Barracks();
        virtual EntityType getType() override;

    /* class SimpleProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
};

class Factory : public Entity,
                //public Upgradable,
                public SimpleProducer
{
    /* class Entity */
    public:
        Factory();
        virtual EntityType getType() override;

    /* class SimpleProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;

    /* class Upgradable
    public:
        virtual bool isUpgradable(GameState& state, EntityType type);
        virtual void upgrade(GameState& state, EntityType to);
        */
};

class Starport : public Entity,
                 //public Upgradable,
                 public SimpleProducer
{
    /* class Entity */
    public:
        Starport();
        virtual EntityType getType() override;
    
    /* class SimpleProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;

    /* class Upgradable 
    public:
        virtual bool isUpgradable(GameState& state, EntityType type) override;
        virtual void upgrade(GameState& state, EntityType to) override;
        */
};
