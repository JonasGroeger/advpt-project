#pragma once

#include "EntityType.hpp"
#include "Entity.hpp"

template<EntityType first>
class SimpleEntity : public Entity
{
public:
    SimpleEntity()
    {
        type = first;
    }
};

