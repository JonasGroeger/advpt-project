#pragma once

#include <entities/SimpleProducer.hpp>
#include "Entity.hpp"
#include "entities/UpgradableProducer.hpp"

class CommandCenter: public Entity,
                     public UpgradableProducer
{
    /* class Entity */
    public:
        CommandCenter();
        virtual EntityType getType() override;

    /* class UpgradableProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
        virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
};

class Barracks: public Entity,
                public UpgradableProducer
{
    /* class Entity */
    public:
        Barracks();
        virtual EntityType getType() override;

    /* class UpgradableProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
        virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
};

class Factory : public Entity,
                public UpgradableProducer
{
    /* class Entity */
    public:
        Factory();
        virtual EntityType getType() override;

    /* class UpgradableProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
        virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
};

class Starport : public Entity,
                 public UpgradableProducer
{
    /* class Entity */
    public:
        Starport();
        virtual EntityType getType() override;
    
    /* class UpgradableProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
        virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
};


class BarracksTechLab : public Entity,
                       public SimpleProducer
{
    /* class Entity */
    public:
        BarracksTechLab();
        virtual EntityType getType() override;
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
};

class BarracksReactor : public Entity,
                        public SimpleProducer
{
    public:
        BarracksReactor();
        virtual EntityType getType() override;
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;

};
