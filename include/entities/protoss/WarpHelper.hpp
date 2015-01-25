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

        int getTimeToFinish()
        {
            return warpTime - doneTime;
        }

        bool isFinished()
        {
            return getTimeToFinish() <= 0;
        }

        void update()
        {
            doneTime++;
        }

        EntityType getType()
        {
            return product;
        }
    };

    enum ArchonFuseType
    {
        NO_FUSE, DOUBLE_HIGH, DOUBLE_DARK, DIFFERENT
    };
    ArchonFuseType archonFuseType = ArchonFuseType::NO_FUSE;

    long maxTime = 0;
    std::vector<WarpTask *> warpTasks;

    virtual void warpBuilding(int duration, EntityType type, GameState &state);


protected:


public :
    WarpHelper()
    {
        type = PROTOSS_WARP_HELPER;
        interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
    }
    virtual ~WarpHelper()
    {
        for (WarpTask* task : warpTasks)
        {
            delete task;
        }
        warpTasks.clear();
    }

    /* Updatable */
    virtual void update(GameState &state);

    /* Producer */
    virtual bool produceEntityIfPossible(EntityType type, GameState &state);

    virtual bool isProducing();
};
