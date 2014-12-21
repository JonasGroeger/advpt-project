#pragma once
#include "EntityType.hpp"

class Entity {
protected:
    EntityType type = NONE;

    //00000001
    unsigned char WORKER_INTERFACE = 1 << 0;
    //00000010
    unsigned char UPDATABLE_INTERFACE = 1 << 1;
    //00000100
    unsigned char UPGRADABLE_INTERFACE = 1 << 2;
    //00001000
    unsigned char PRODUCER_INTERFACE = 1 << 3;

    /*
    holds all implemented interfaces
    has to be set in the constructor of an instance
    for example interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE
    means that this entity implements those two interfaces
    */
    unsigned char interfaceBitmask = 0;

public:
    virtual EntityType getType(){ return this->type; };
    virtual void setType(EntityType newType){ this->type = newType; }
    virtual ~Entity() {};

    /*
    The following methods will return if a specific interface is implemented by this instance
    addEntity inside GameState has to check if a entity is of a specific type by calling them
    */
    bool isWorker();
    bool isProducer();
    bool isUpdatable();
    bool isUpgradable();

    EntityType getRace();
    static EntityType typeToRace(EntityType type);
};
