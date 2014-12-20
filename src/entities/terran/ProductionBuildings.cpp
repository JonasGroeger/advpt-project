#include "entities/terran/ProductionBuildings.hpp"
#include "GameState.hpp"

CommandCenter::CommandCenter()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | UPGRADABLE_INTERFACE;
}

bool CommandCenter::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    switch(type){
        case EntityType::TERRAN_SCV:
            if(state.hasEnough(50, 0, 1))
            {
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                this->state = UPState::PRODUCING;
                currentProgress = 0;
                maxProgress = 17;
                product = type;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

// TODO
bool CommandCenter::upgradeIfPossible(EntityType type, GameState &state)
{
    switch (type)
    {
        case TERRAN_ORBITAL_COMMAND:
            if (state.hasEnoughMinerals(150))
            {
                this->state = UPState::UPGRADING;
                product = type;
                maxProgress = 35;
                state.consumeEnoughMinerals(150);
                return true;
            }
            break;
        case TERRAN_PLANETARY_FORTRESS:
            if (state.hasEnough(150, 150, 0) && state.hasEntity(TERRAN_ENGINEERING_BAY))
            {
                product = type;
                maxProgress = 50;
                state.consumeEnoughMinerals(150);
                state.consumeEnoughVespine(150);
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}




Barracks::Barracks()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
    type = TERRAN_BARRACKS;
}

bool Barracks::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy())
    {
        return false;
    }

    EntityType barrackType = this->getType();
    unsigned long minerals = 0;
    unsigned long gas = 0;
    unsigned long supply = 0;
    unsigned long time = 0;

    switch(type){
        case EntityType::TERRAN_MARINE:
            if(state.hasEnough(50, 0, 1)){
                state.consumeEnoughMinerals(50);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                this->state = UPState::PRODUCING;
                currentProgress = 0;
                maxProgress = 25;
                product = type;
                return true;
            }
            break;
        case EntityType::TERRAN_MARAUDER:
            if(barrackType != TERRAN_BARRACKS_TECH_LAB){
                return false;
            }
            minerals = 100;
            gas = 25;
            time = 30;
            supply = 2;
            break;
        case EntityType::TERRAN_REAPER:
            if(barrackType != TERRAN_BARRACKS_TECH_LAB){
                return false;
            }
            minerals = 50;
            gas = 50;
            time = 45;
            supply = 1;
            break;

        case EntityType::TERRAN_GHOST:
            if(barrackType != TERRAN_BARRACKS_TECH_LAB){
                return false;
            }
            if(!state.hasEntity(EntityType::TERRAN_GHOST_ACADEMY)){
                return false;
            }
            minerals = 200;
            gas = 100;
            time = 40;
            supply = 2;
            break;
        default:
            return false;
    }

    if(state.hasEnough(minerals, gas, supply)){
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        state.notifyEntityIsBeingProduced(type);
        this->state = UPState::PRODUCING;
        currentProgress = 0;
        maxProgress = time;
        product = type;
        return true;
    }

    return false;
}

bool Barracks::upgradeIfPossible(EntityType type, GameState &state)
{
    if(isBusy()
            || this->getType() == TERRAN_BARRACKS_TECH_LAB
            || this->getType() == TERRAN_BARRACKS_REACTOR)
    {
        return false;
    }

    unsigned int minerals = 0;
    unsigned int gas = 0;
    unsigned int time = 0;

    switch (type)
    {
        case TERRAN_BARRACKS_TECH_LAB:
            minerals = 50;
            gas = 25;
            time = 25;
            break;
        case TERRAN_BARRACKS_REACTOR:
            minerals = 50;
            gas = 50;
            time = 50;
            break;
        default:
            return false;
    }

    if(state.hasEnough(minerals, gas, 0)){
        this->state = UPState::UPGRADING;
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        product = type;
        currentProgress = 0;
        maxProgress = time;
        return true;
    }

    return false;
}

void Barracks::update(GameState& state){
    switch(this->state)
    {
        case UPState::PRODUCING:
            currentProgress ++;

            if (currentProgress >= maxProgress)
            {
                state.addEntity(product, 1);
                this->state = UPState::IDLE;
            }
            break;
        case UPState::UPGRADING:
            currentProgress ++;

            if (currentProgress >= maxProgress)
            {
                //just switch our own entitytype and notify gamestate of the new type
                Entity* ourEntity = dynamic_cast<Entity*>(this);
                ourEntity->setType(product);
                state.setAvailableEntityType(product);
                //if we have a reactor as upgrade add a second instance to gamestate since
                //reactor doubles the capacity
                if(product == TERRAN_BARRACKS_REACTOR){
                    Entity* secondBarrack = new Barracks();
                    secondBarrack->setType(TERRAN_BARRACKS_REACTOR);
                    state.addCreatedEntity(secondBarrack);
                }
                //gameState.addEntity(product, 1);
                this->state = UPState::IDLE;
            }
        default:
            return;
    }
}




Factory::Factory()
{
    interfaceBitmask = UPDATABLE_INTERFACE | UPGRADABLE_INTERFACE | PRODUCER_INTERFACE;
    type = TERRAN_FACTORY;
}

bool Factory::produceEntityIfPossible(EntityType type, GameState &state)
{
    if (isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, supply = 0;
    switch(type)
    {
        case EntityType::TERRAN_HELLION:
            minerals = 100;
            supply = 2;
            maxProgress = 30;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        this->state = UPState::PRODUCING;
        currentProgress = 0;
        product = type;
        return true;
    }

    return false;
}

bool Factory::upgradeIfPossible(EntityType type, GameState &state)
{
    // TODO
    return false;
}





Starport::Starport()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
    type = TERRAN_STARPORT;
}

bool Starport::produceEntityIfPossible(EntityType type, GameState &state)
{
    if(isBusy())
    {
        return false;
    }

    int minerals = 0, gas = 0, supply = 0;
    switch(type)
    {
        case EntityType::TERRAN_MEDIVAC:
            minerals = 100;
            gas = 100;
            supply = 2;
            maxProgress = 42;
            break;
        case EntityType::TERRAN_VIKING:
            minerals = 150;
            gas = 75;
            supply = 2;
            maxProgress = 42;
            break;
        default:
            return false;
    }

    if (state.hasEnough(minerals, gas, supply))
    {
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.consumeEnoughSupply(supply);
        this->state = UPState::PRODUCING;
        currentProgress = 0;
        product = type;
        return true;
    }

    return false;
}

bool Starport::upgradeIfPossible(EntityType type, GameState &state)
{
    // TODO
    return false;
}
