#include <entities/protoss/WarpHelper.hpp>

WarpHelper* WarpHelper::_instance = 0;

/* Updatable */
void WarpHelper::update(GameState &state)
{
    auto it = warpTasks.begin();
    auto end = warpTasks.end();
    while (it != end) {
        // do some stuff
        if((*it)->isFinished()){
            state.addEntity((*it)->getType(), 1);

            WarpTask* toDelete = *it;
            warpTasks.erase(it);
            delete(toDelete);
        }
        else{
            (*it)->update();
        }
        it++;
    }
}

bool WarpHelper::produceEntityIfPossible(EntityType type, GameState& state)
{
    int minerals = 0, gas = 0, time = 0;
    switch(type){
        case PROTOSS_NEXUS:
            minerals = 400;
            time = 100;
            break;
        case PROTOSS_PYLON:
            minerals = 100;
            time = 25;
            break;
        case PROTOSS_ASSIMILATOR:
            minerals = 75;
            time = 30;
            break;
        case PROTOSS_GATEWAY:
            minerals = 150;
            time = 65;
            break;
        case PROTOSS_FORGE:
            minerals = 150;
            time = 45;
            break;
        case PROTOSS_CYBERNETICS_CORE:
            minerals = 150;
            time = 50;
            break;
        case PROTOSS_PHOTON_CANNON:
            minerals = 150;
            time = 40;
            break;
        case PROTOSS_ROBOTICS_FACILITY:
            minerals = 200;
            gas = 100;
            time = 60;
            break;
        case PROTOSS_TWILIGHT_COUNCIL:
            minerals = 150;
            gas = 100;
            time = 50;
            break;
        case PROTOSS_STARGATE:
            minerals = 150;
            gas = 150;
            time = 60;
            break;
        case PROTOSS_ROBOTICS_BAY:
            minerals = 200;
            gas = 200;
            time = 65;
            break;
        case PROTOSS_TEMPLAR_ARCHIVES:
            minerals = 150;
            gas = 200;
            time = 50;
            break;
        case PROTOSS_DARK_SHRINE:
            minerals = 100;
            gas = 250;
            time = 100;
            break;
        case PROTOSS_FLEET_BEACON:
            minerals = 300;
            gas = 200;
            time = 60;
            break;
        default:
            return false;
    }

    if(state.hasEnough(minerals, gas, 0)){
        state.consumeEnough(minerals, gas, 0);
        warpBuilding(time, type, state);
        return true;
    }
    return false;
}

long WarpHelper::getTimeToFinish()
{
    for(auto task : warpTasks){
        maxTime = (task->getTimeToFinish() > maxTime)
                ? task->getTimeToFinish()
                : maxTime;
    }

    return maxTime;
}


void WarpHelper::warpBuilding(int duration, EntityType type, GameState &state)
{
    WarpTask* newTask = new WarpTask(duration, type);
    warpTasks.push_back(newTask);
    state.notifyEntityIsBeingProduced(type);
}
