#include "Entity.hpp"

/*
The following methods will return if a specific interface is implemented by this instance
addEntity inside GameState has to check if a entity is of a specific type by calling them
*/
bool Entity::isWorker(){
        bool hasInterface = (interfaceBitmask & WORKER_INTERFACE);
        return hasInterface;
}

bool Entity::isProducer() {
        bool hasInterface = (interfaceBitmask & PRODUCER_INTERFACE);
        return hasInterface;
};

bool Entity::isUpdatable() {
        bool hasInterface = (interfaceBitmask & UPDATABLE_INTERFACE);
        return hasInterface;
};

bool Entity::isUpgradable() {
        bool hasInterface = (interfaceBitmask & UPGRADABLE_INTERFACE);
        return hasInterface;
};

EntityType Entity::getRace()
{
        return typeToRace(this->type);
}

EntityType Entity::typeToRace(EntityType type)
{
        if (NONE < type && type <= TERRAN)
        {
                return TERRAN;
        }
        else if (TERRAN < type && type <= ZERG)
        {
                return ZERG;
        }
        else if (ZERG < type && type <= PROTOSS)
        {
                return PROTOSS;
        }

        return NONE; // should never happen
}
