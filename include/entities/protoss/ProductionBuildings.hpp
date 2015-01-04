#pragma once

#include <entities/SimpleProducer.hpp>
#include "Entity.hpp"
#include "entities/UpgradableProducer.hpp"

class Nexus : public Entity,
              public SimpleProducer
{
    /* class Entity */
public:
    Nexus()
    {
        interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
        type = PROTOSS_NEXUS;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
};

class Gateway : public Entity,
                public SimpleProducer
{
    /* class Entity */
public:
    Gateway()
    {
        interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
        type = PROTOSS_GATEWAY;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
};

class RoboticsFacility : public Entity,
                         public SimpleProducer
{
    /* class Entity */
public:
    RoboticsFacility()
    {
        interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
        type = PROTOSS_ROBOTICS_FACILITY;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
};

class Stargate : public Entity,
                 public SimpleProducer
{
    /* class Entity */
public:
    Stargate()
    {
        interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
        type = PROTOSS_STARGATE;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;
};

