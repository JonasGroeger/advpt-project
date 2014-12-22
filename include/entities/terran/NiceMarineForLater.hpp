#include <GameState.hpp>
#include "Entity.hpp"

class Marine : public Entity
{
private:
    const unsigned int requiredMinerals = 50;
    const unsigned int requiredGas = 0;
    const unsigned int requiredSupply = 2;
    const unsigned int requiredTime = 25;
public:
    EntityType getType()
    {
        return EntityType::TERRAN_MARINE;
    };

    bool isProducable(GameState &gameState)
    {
        bool result = true;
        if (!gameState.hasEnough(requiredMinerals, requiredGas, requiredSupply))
        {
            return false;
        }

        // TODO: If required, add more requirements according to gamestate

        return result;
    };

    vector<EntityType> resolvedDependencies()
    {
        return new vector<EntityType>{
                EntityType::TERRAN_MARINE
        };
    }
};
