#pragma once

#include <entities/SimpleProducer.hpp>
#include "Entity.hpp"
#include "entities/UpgradableProducer.hpp"

class Hatchery : public Entity,
                 public UpgradableProducer
{
    /* class Entity */
public:
    Hatchery(){
        interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | UPGRADABLE_INTERFACE;
        type = ZERG_HATCHERY;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
    virtual bool upgradeIfPossible(EntityType type, GameState &state) override;
    //overrides update here to produce larva
    virtual void update(GameState& state) override;
};


class NydusNetwork : public Entity,
                     public SimpleProducer
{
    /* class Entity */
public:
    NydusNetwork(){
        interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
        type = ZERG_NYDUS_NETWORK;
    }

    /* class UpgradableProducer */
public:
    virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
};
