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
    /* class UpgradableProducer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
        virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
};
