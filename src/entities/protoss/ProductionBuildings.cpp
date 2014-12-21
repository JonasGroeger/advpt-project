#include "entities/protoss/ProductionBuildings.hpp"
#include "GameState.hpp"

bool Nexus::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    //TODO
    return false;
}

bool Gateway::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    //TODO
    return false;
}

bool RoboticsFacility::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    //TODO
    return false;
}

bool Stargate::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    //TODO
    return false;
}
