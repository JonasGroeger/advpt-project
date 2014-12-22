#pragma once

#include <iostream>

#include "Entity.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class Drone : public Entity,
              public Worker,
              public Updatable,
              public Upgradable
{
private:
    int currentProgress = 0;
    int maxProgress = 0;
    bool morphing = false;

    /* class Entity */
public:
    Drone();

    /* class Updatable */
public:
    void update(GameState &state) final;

    /* class Upgradable */
public:
    bool upgradeIfPossible(EntityType type, GameState &state) final;
};
