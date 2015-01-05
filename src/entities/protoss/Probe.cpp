#include "entities/protoss/Probe.hpp"

Probe::Probe()
{
    interfaceBitmask = UPDATABLE_INTERFACE | WORKER_INTERFACE;
    type = PROTOSS_PROBE;
}

/**
* A probe has no downtime for producing a building and thus does
* not need to handle the Producer interface. It is never producing.
*/
void Probe::update(GameState &state)
{
    switch (this->getTypeOfWork())
    {
        case TypeOfWork::Idle:
            break;
        case TypeOfWork::Minerals:
            state.addMineralsWithFactor(0.7 * GameState::FACTOR);
            break;
        case TypeOfWork::Vespine:
            state.addVespineWithFactor(0.35 * GameState::FACTOR);
            break;
        case TypeOfWork::Producing:
            break;
    }
}
