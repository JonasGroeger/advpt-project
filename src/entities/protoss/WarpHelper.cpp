#include <entities/protoss/WarpHelper.hpp>

WarpHelper* WarpHelper::_instance = 0;

/* Updatable */
void WarpHelper::update(GameState &state){
    auto task = warpTasks.begin();
    while (task != std::end(warpTasks)) {
        // do some stuff
        if((*task)->isFinished()){
            state.addEntity((*task)->getType(), 1);
            warpTasks.erase(task);
        }
        else{
            (*task)->update();
        }
        task++;
    }
}

void WarpHelper::warpBuilding(int duration, EntityType type, GameState &state){
    WarpTask* newTask = new WarpTask(duration, type);
    warpTasks.push_back(newTask);
    state.notifyEntityIsBeingProduced(type);
}
