#pragma once

#include <EntityType.hpp>
#include <GameState.hpp>
#include <vector>

class WarpHelper : public Updatable,
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

        bool isFinished()
        {
            return warpTime >= doneTime;
        }

        bool update()
        {
            doneTime++;
        }

        EntityType getType(){
            return product;
        }
    };
    std::vector<WarpTask*> warpTasks;
    static WarpHelper *_instance;

protected:
    WarpHelper(){

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
    virtual long getTimeToFinish() {return 0;}

};



