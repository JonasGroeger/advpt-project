#pragma once

#include "Entity.hpp"

class Marine: public Entity
{
    /* class Entity */
public:
    virtual EntityType getType();
};