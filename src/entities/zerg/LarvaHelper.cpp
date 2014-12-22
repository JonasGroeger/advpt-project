#include "entities/zerg/LarvaHelper.hpp"
#include<iostream>

LarvaHelper::LarvaHelper()
{
    setType(ZERG_LARVA_HELPER);
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

void LarvaHelper::update(GameState& state)
{
    currentTime ++;

    if (morphings.size() == 0)
    {
        return;
    }

    MorphObject *next = morphings.top();
    while (next->finishTime >= currentTime)
    {
        // Zerglings are always produced in pairs of two
        if (next->product == ZERG_ZERGLING)
        {
            state.addEntity(next->product, 2);
        }
        else
        {
            state.addEntity(next->product, 1);
        }

        morphings.pop();
        if (morphings.size() == 0)
        {
            break;
        }
        next = morphings.top();
    }
}

bool LarvaHelper::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (currentLarva <= 0)
    {
        return false;
    }

    int minerals = 0, gas = 0, time = 0, supply = 0;
    switch (type)
    {
        case ZERG_DRONE:
            minerals = 50;
            time = 17;
            supply = 1;
            break;
        case ZERG_OVERLORD:
            minerals = 100;
            time = 25;
            supply = 0; // TODO this should add supply in GameState
            break;
        case ZERG_ZERGLING:
            if (!state.hasEntity(ZERG_SPAWNING_POOL))
            {
                return false;
            }
            minerals = 50;
            time = 24;
            supply = 1;
            break;
        case ZERG_HYDRALISK:
            if (!state.hasEntity(ZERG_HYDRALISK_DEN))
            {
                return false;
            }
            minerals = 100;
            gas = 50;
            time = 33;
            supply = 2;
            break;
        case ZERG_ULTRALISK:
            if (!state.hasEntity(ZERG_ULTRALIK_CAVERN))
            {
                return false;
            }
            minerals = 300;
            gas = 200;
            time = 55;
            supply = 6;
            break;
        case ZERG_INFESTOR:
            if (!state.hasEntity(ZERG_INFESTATION_PIT))
            {
                return false;
            }
            minerals = 100;
            gas = 150;
            time = 50;
            supply = 2;
            break;
        case ZERG_ROACH:
            if (!state.hasEntity(ZERG_ROACH_WARREN))
            {
                return false;
            }
            minerals = 75;
            gas = 25;
            time = 27;
            supply = 2;
            break;
        case ZERG_CORRUPTOR:
            if (!state.hasEntity(ZERG_SPIRE))
            {
                return false;
            }
            minerals = 150;
            gas = 100;
            time = 40;
            supply = 2;
            break;
        case ZERG_MUTALISK:
            if (!state.hasEntity(ZERG_SPIRE))
            {
                return false;
            }
            minerals = 100;
            gas = 100;
            time = 33;
            supply = 2;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        state.consumeEnough(minerals, gas, supply);
        state.notifyEntityIsBeingProduced(type);
        MorphObject *obj = new MorphObject();
        obj->finishTime = currentTime + time;
        obj->product = type;
        morphings.push(obj);
        currentLarva --;
        maxTime = (obj->finishTime > maxTime) ? (obj->finishTime) : (maxTime);
        
        return true;
    }

    return false;
}

bool LarvaHelper::isProducing()
{
    if (morphings.size() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void LarvaHelper::increaseLarva()
{
    currentLarva ++;
    if (currentLarva > maxLarva)
    {
        currentLarva = maxLarva;
    }
}

void LarvaHelper::addMaxLarva(unsigned long amount)
{
    maxLarva += amount;
}
