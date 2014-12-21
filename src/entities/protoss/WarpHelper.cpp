#include <entities/protoss/WarpHelper.hpp>

WarpHelper* WarpHelper::_instance = 0;

/* Updatable */
void WarpHelper::update(GameState &state){
    for(auto task : warpTasks){
        if(task->isFinished()){
            state.addEntity(task->getType(), 1);
        }
        else{
            task->update();
        }
    }
}

void WarpHelper::warpBuilding(int duration, EntityType type){
    WarpTask* newTask = new WarpTask(duration, type);
    warpTasks.push_back(newTask);
}
