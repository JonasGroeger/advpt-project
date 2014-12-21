#pragma once

#include <EntityType.hpp>
#include <GameState.hpp>
#include <vector>

class WarpHelper : public Updatable,
                   public Entity,
                   public Producer
{

private :
    class WarpTask
    {
    private :
        int warpTime = 0;
        int doneTime = 0;
        EntityType product = NONE;
    public :
        WarpTask(int duration, EntityType type)
        {
            warpTime = duration;
            product = type;
        }

        int getTimeToFinish(){
            return warpTime - doneTime;
        }

        bool isFinished()
        {
            return getTimeToFinish() <= 0;
        }

        bool update()
        {
            doneTime++;
        }

        EntityType getType(){
            return product;
        }
    };

    long maxTime = 0;
    std::vector<WarpTask*> warpTasks;
    static WarpHelper *_instance;

protected:
    WarpHelper(){
        this->type = PROTOSS_WARP_HELPER;
    }

public :
    static WarpHelper *Instance()
    {
        if (_instance == 0)
        {
            _instance = new WarpHelper();
        }
        return _instance;
    }

    virtual void warpBuilding(int duration, EntityType type, GameState& state);

    /* Updatable */
    virtual void update(GameState &state);

    /* Producer */
    virtual bool produceEntityIfPossible(EntityType type, GameState& state);
    virtual long getTimeToFinish();

};
