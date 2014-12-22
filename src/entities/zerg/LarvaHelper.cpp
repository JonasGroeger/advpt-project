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
        state.addEntity(next->product, 1);
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
        
        return true;
    }

    return false;
}

long LarvaHelper::getTimeToFinish()
{
    return 0; // TODO
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
