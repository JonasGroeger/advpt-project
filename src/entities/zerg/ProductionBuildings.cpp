#include <entities/zerg/ProductionBuildings.hpp>
#include <GameState.hpp>

bool Hatchery::produceEntityIfPossible(EntityType type, GameState& state)
{
    if (isBusy()){
        return false;
    }

    if(type == ZERG_QUEEN){
            // TODO
    }
    return false;
}
bool Hatchery::upgradeIfPossible(EntityType type, GameState &state)
{

    if(isBusy() || this->getType() == ZERG_HIVE){
        return false;
    }

    if(type == ZERG_LAIR && this->getType() == ZERG_HATCHERY)
    {
        if(state.hasEnough(150, 100, 0)){
            state.consumeEnough(150, 100, 0);
            product = type;
            maxProgress = 80;
            currentProgress = 0;
            this->state = UPState::UPGRADING;
            return true;
        }
    }
    else if(type == ZERG_HIVE && this->getType() == ZERG_LAIR)
    {
        if(state.hasEnough(200, 150, 0)){
            state.consumeEnough(200, 150, 0);
            product = type;
            maxProgress = 100;
            currentProgress = 0;
            this->state = UPState::UPGRADING;
            return true;
        }
    }

    return false;
}

void Hatchery::update(GameState& state){

    //larva production every 15 seconds
    if(state.getSimulationTime() % 15 == 0){
        state.increaseLarva();
    }

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
                this->setType(product);
                state.setAvailableEntityType(product);

                this->state = UPState::IDLE;
                printBuildEndMessage(product, state.getSimulationTime());

            }
        default:
            return;
    }
}


bool NydusNetwork::produceEntityIfPossible(EntityType type, GameState& state){

    if(isBusy()){
        return false;
    }

    if(type == ZERG_NYDUS_WORM){
        if(state.hasEnough(100, 100, 0)){
            state.consumeEnough(100, 100, 0);
            maxProgress = 20;
            currentProgress = 0;
            product = type;
            this->state = UPState::PRODUCING;
            return true;
        }
    }
    return false;
}
