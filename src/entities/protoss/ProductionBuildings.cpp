#include "entities/protoss/ProductionBuildings.hpp"
#include "GameState.hpp"

bool Nexus::produceEntityIfPossible(EntityType type, GameState &state)
{
    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, time = 0, supply = 0;
    switch (type)
    {
        case PROTOSS_PROBE:
            minerals = 50;
            time = 17;
            supply = 1;
            break;

        case PROTOSS_MOTHERSHIP:
            if (state.hasEntity(PROTOSS_MOTHERSHIP) || !state.hasEntity(PROTOSS_FLEET_BEACON))
            {
                //only allowed once
                return false;
            }
            minerals = 400;
            gas = 400;
            time = 160;
            supply = 8;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        currentProgress = 0;
        maxProgress = time;
        state.consumeEnough(minerals, gas, supply);
        product = type;
        this->state = UPState::PRODUCING;
        return true;
    }

    return false;
}

bool Gateway::produceEntityIfPossible(EntityType type, GameState &state)
{
    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, time = 0, supply = 0;
    bool hit = false;

    switch (type)
    {
        case PROTOSS_ZEALOT:
            minerals = 100;
            time = 38;
            supply = 2;
            hit = true;
            break;
        case PROTOSS_HIGH_TEMPLAR:
            if (!state.hasEntity(PROTOSS_TEMPLAR_ARCHIVES))
            {
                return false;
            }
            minerals = 50;
            gas = 150;
            time = 55;
            supply = 2;
            hit = true;
            break;
        case PROTOSS_DARK_TEMPLAR:
            if (!state.hasEntity(PROTOSS_DARK_SHRINE))
            {
                return false;
            }
            minerals = 125;
            gas = 125;
            time = 55;
            supply = 2;
            hit = true;
            break;
        default:
            break;
    }
    if (state.hasEntity(PROTOSS_CYBERNETICS_CORE) && !hit)
    {
        switch (type)
        {
            case PROTOSS_STALKER:
                minerals = 125;
                gas = 50;
                time = 42;
                supply = 2;
                hit = true;
                break;
            case PROTOSS_SENTRY:
                minerals = 50;
                gas = 100;
                time = 37;
                supply = 2;
                hit = true;
                break;
            default:
                return false;
        }
    }


    if (state.hasEnough(minerals, gas, supply) && hit)
    {
        currentProgress = 0;
        maxProgress = time;
        state.consumeEnough(minerals, gas, supply);
        product = type;
        this->state = UPState::PRODUCING;
        return true;
    }

    return false;
}

bool RoboticsFacility::produceEntityIfPossible(EntityType type, GameState &state)
{

    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, time = 0, supply = 0;
    switch (type)
    {
        case PROTOSS_WARP_PRISM:
            minerals = 200;
            time = 50;
            supply = 2;
            break;
        case PROTOSS_IMMORTAL:
            minerals = 250;
            gas = 100;
            time = 55;
            supply = 4;
            break;
        case PROTOSS_OBSERVER:
            minerals = 25;
            gas = 75;
            time = 30;
            supply = 1;
            break;
        case PROTOSS_COLOSSUS:
            if (!state.hasEntity(PROTOSS_ROBOTICS_BAY))
            {
                return false;
            }
            minerals = 300;
            gas = 200;
            time = 75;
            supply = 6;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        currentProgress = 0;
        maxProgress = time;
        state.consumeEnough(minerals, gas, supply);
        product = type;
        this->state = UPState::PRODUCING;
        return true;
    }

    return false;
}

bool Stargate::produceEntityIfPossible(EntityType type, GameState &state)
{
    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, time = 0, supply = 0;
    switch (type)
    {
        case PROTOSS_PHOENIX:
            minerals = 150;
            gas = 100;
            time = 35;
            supply = 2;
            break;
        case PROTOSS_VOID_RAY:
            minerals = 250;
            gas = 150;
            time = 60;
            supply = 3;
            break;
        case PROTOSS_CARRIER:
            if (!state.hasEntity(PROTOSS_FLEET_BEACON))
            {
                return false;
            }
            minerals = 350;
            gas = 250;
            time = 120;
            supply = 6;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        currentProgress = 0;
        maxProgress = time;
        state.consumeEnough(minerals, gas, supply);
        product = type;
        this->state = UPState::PRODUCING;
        return true;
    }

    return false;
}
