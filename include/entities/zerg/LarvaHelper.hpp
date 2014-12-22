#pragma once

#include <queue>
#include <vector>
#include <iostream>

#include "Entity.hpp"
#include "Updatable.hpp"
#include "EntityType.hpp"
#include "GameState.hpp"
#include "entities/Producer.hpp"

class LarvaHelper : public Entity,
                    public Updatable,
                    public Producer
{
    private:
        struct MorphObject
        {
            int finishTime;
            EntityType product;
        };

        struct MorphObjectComparator
        {
            bool operator() (MorphObject *lhs, MorphObject *rhs)
            {
                return lhs->finishTime > rhs->finishTime;
            }
        };

        int currentTime = 0;
        int maxTime = 0;
        int numHatcheries = 0; // TODO GameState should increase this in addEntity
        int currentLarva = 0; // TODO GameState should increase this in addEntity

        std::priority_queue<MorphObject*, std::vector<MorphObject*>, MorphObjectComparator> morphings;

    public:
        LarvaHelper();
        void update(GameState& state) final;
        virtual bool produceEntityIfPossible(EntityType type, GameState& state) override;
        virtual bool isProducing() override;
        virtual void applyChronoBoost() {}

        void increaseLarva();
        void addHatchery();

        int getMaxLarva();
        int getCurrentLarva();
};
