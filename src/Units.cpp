#include "Units.hpp"


SCV::SCV()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | WORKER_INTERFACE;
}

EntityType SCV::getType()
{
    return TERRAN_SCV;
}

void SCV::update(GameState& state)
{
    Worker* w = dynamic_cast<Worker*>(this);
    switch (w->getTypeOfWork()){
        case TypeOfWork::Idle:
            break;
        case TypeOfWork::Minerals:
            state.addMineralsWithFactor(0.7 * GameState::FACTOR);
            break;
        case TypeOfWork::Vespine:
            state.addVespineWithFactor(0.35 * GameState::FACTOR);
            break;
        case TypeOfWork::Producing:
            currentProgress ++;
            if (currentProgress >= maxProgress)
            {
                state.addEntity(product, 1);
                if (state.hasOpenVespeneSlot())
                {
                    w->setTypeOfWork(TypeOfWork::Vespine);
                }
                else
                {
                    w->setTypeOfWork(TypeOfWork::Minerals);
                }
            }
            break;
    }
}

bool SCV::produceEntityIfPossible(EntityType type, GameState& state)
{
    Worker* w = dynamic_cast<Worker*>(this);
    TypeOfWork current = w->getTypeOfWork();
    if(current == TypeOfWork::Producing)
    {
        return false;
    }

    //to check at the end
    unsigned long minerals = 0;
    unsigned long gas = 0;

    switch(type)
    {
        case EntityType::TERRAN_SUPPLY_DEPOT:
            maxProgress = 30;
            minerals = 100;
            break;
        case EntityType::TERRAN_REFINERY:
            maxProgress = 30;
            minerals = 75;
            break;
        case EntityType::TERRAN_BARRACKS:
            maxProgress = 65;
            minerals = 150;
            break;
        case EntityType::TERRAN_COMMAND_CENTER:
            maxProgress = 100;
            minerals = 400;
            break;
        case EntityType::TERRAN_GHOST_ACADEMY:
            maxProgress = 40;
            minerals = 150;
            gas = 50;
            break;
        case EntityType::TERRAN_BUNKER:
            maxProgress = 40;
            minerals = 100;
            break;
        case EntityType::TERRAN_FACTORY:
            maxProgress = 60;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_ENGINEERING_BAY:
            maxProgress = 30;
            minerals = 125;
            break;
        case EntityType::TERRAN_ARMORY:
            maxProgress = 35;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_STARPORT:
            maxProgress = 50;
            minerals = 150;
            gas = 100;
            break;
        case EntityType::TERRAN_FUSION_CORE:
            maxProgress = 65;
            minerals = 150;
            gas = 150;
            break;
        case EntityType::TERRAN_SENSOR_TOWER:
            maxProgress = 25;
            minerals = 125;
            gas = 100;
            break;
        case EntityType::TERRAN_MISSILE_TURRET:
            maxProgress = 25;
            minerals = 100;
            break;
        default:
            return false;
    }

    //we can produce that thing
    if (state.hasEnough(minerals, gas, 0))
    {
        w->setTypeOfWork(TypeOfWork::Producing);
        state.consumeEnoughMinerals(minerals);
        state.consumeEnoughVespine(gas);
        state.notifyEntityIsBeingProduced(type);
        product = type;
        currentProgress = 0;
        return true;
    }
    return false;
}

long SCV::getTimeToFinish()
{
    Worker* w = dynamic_cast<Worker*>(this);

    if (w->getTypeOfWork() == TypeOfWork::Producing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

void SCV::applyChronoBoost()
{
}

EntityType SupplyDepot::getType()
{
    return TERRAN_SUPPLY_DEPOT;
}

EntityType Refinery::getType()
{
    return TERRAN_REFINERY;
}

EntityType Hellion::getType()
{
    return TERRAN_HELLION;
}

EntityType BattleCruiser::getType()
{
    return TERRAN_BATTLECRUISER;
}

/* TODO: IMPLEMENT the functions below, as they are only dummies */



EntityType Armory::getType() {
	return TERRAN_ARMORY;
}

EntityType Banshee::getType() {
	return TERRAN_BANSHEE;
}

EntityType Bunker::getType() {
	return TERRAN_BUNKER;
}

EntityType Reaper::getType() {
	return TERRAN_REAPER;
}

EntityType Ghost::getType() {
	return TERRAN_GHOST;
}

EntityType GhostAcademy::getType() {
	return TERRAN_GHOST_ACADEMY;
}

EntityType FactoryTechLab::getType() {
	return TERRAN_FACTORY_TECH_LAB;
}

EntityType FactoryReactor::getType() {
	return TERRAN_FACTORY_REACTOR;
}

EntityType StarportTechLab::getType()  {
	return TERRAN_STARPORT_TECH_LAB;
}

EntityType StarportReactor::getType() {
	return TERRAN_STARPORT_REACTOR;
}

EntityType Raven::getType() {
	return TERRAN_RAVEN;
}

EntityType FusionCore::getType() {
	return TERRAN_FUSION_CORE;
}

EntityType Viking::getType() {
	return TERRAN_VIKING;
}

EntityType Medivac::getType() {
	return TERRAN_MEDIVAC;
}

EntityType SiegeTank::getType() {
	return TERRAN_SIEGE_TANK;
}

EntityType Thor::getType() {
	return TERRAN_THOR;
}

EntityType OrbitalCommand::getType() {
	return TERRAN_ORBITAL_COMMAND;
}

EntityType EngineeringBay::getType() {
	return TERRAN_ENGINEERING_BAY;
}


EntityType PlanetaryFortress::getType() {
	return TERRAN_PLANETARY_FORTRESS;
}

EntityType MissileTurret::getType() {
	return TERRAN_MISSILE_TURRET;
}

EntityType SensorTower::getType() {
	return TERRAN_SENSOR_TOWER;
}


bool PlanetaryFortress::produceEntityIfPossible(EntityType type, GameState &state)
{
	return true;
}


long PlanetaryFortress::getTimeToFinish() {
	return 0;
}
void PlanetaryFortress::applyChronoBoost() 
{
}

