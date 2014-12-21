#include "entities/protoss/Probe.hpp"

Probe::Probe()
{
    interfaceBitmask = UPDATABLE_INTERFACE | WORKER_INTERFACE;
    type = PROTOSS_PROBE;
}

void Probe::update(GameState &state)
{
    switch (this->getTypeOfWork())
    {
        case TypeOfWork::Minerals:
            state.addMineralsWithFactor(0.7 * GameState::FACTOR);
            break;
        case TypeOfWork::Vespine:
            state.addVespineWithFactor(0.35 * GameState::FACTOR);
            break;
        case TypeOfWork::Idle:
        case TypeOfWork::Producing:
            break;
    }
}
