#pragma once
#include "EntityType.hpp"

class Entity
{
private:
    EntityType type;
public:
    virtual EntityType getType();
    virtual ~Entity();
};
