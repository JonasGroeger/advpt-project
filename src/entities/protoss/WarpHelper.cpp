#include <entities/protoss/WarpHelper.hpp>

WarpHelper* WarpHelper::_instance = 0;

/* Updatable */
void WarpHelper::update(GameState &state)
{
    auto it = warpTasks.begin();
    while (it != std::end(warpTasks)) {
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
    //TODO
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
