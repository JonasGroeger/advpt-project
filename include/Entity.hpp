#pragma once
#include "EntityType.hpp"

class Entity {
private:
    EntityType type;

protected:
    //00000001
    unsigned char ENTITY_INTERFACE = 1 << 0;
    //00000010
    unsigned char UPDATABLE_INTERFACE = 1 << 1;
    //00000100
    unsigned char UPGRADABLE_INTERFACE = 1 << 2;
    //00001000
    unsigned char PRODUCER_INTERFACE = 1 << 3;
    //holds all
    unsigned char interfaceBitmask = 0;

public:
    virtual EntityType getType();
    virtual ~Entity();

    bool isProducer() {
        bool hasInterface = (interfaceBitmask & PRODUCER_INTERFACE);
        return hasInterface;
    };

    bool isUpdatable() {
        bool hasInterface = (interfaceBitmask & UPDATABLE_INTERFACE);
        return hasInterface;
    };

    bool isUpgradable() {
        bool hasInterface = (interfaceBitmask & UPGRADABLE_INTERFACE);
        return hasInterface;
    };
};
