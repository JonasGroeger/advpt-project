#include "Units.hpp"

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
                isProducing = false;
                w->setTypeOfWork(TypeOfWork::Minerals);
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

EntityType CommandCenter::getType()
{
    return TERRAN_COMMAND_CENTER;
}

void CommandCenter::update(GameState& state)
{
    if (isProducing)
    {
        currentProgress ++;

        if (currentProgress >= maxProgress)
        {
            state.addEntity(TERRAN_SCV, 1);
            isProducing = false;
        }
    }
}

bool CommandCenter::produceEntityIfPossible(EntityType type, GameState& state)
{
    if(isProducing){
        return false;
    }

    switch(type){
        case EntityType::TERRAN_SCV:
            if(state.hasEnough(50, 0, 1)){
                state.consumeEnoughMinerals(50);
                state.consumeEnoughVespine(0);
                state.consumeEnoughSupply(1);
                state.notifyEntityIsBeingProduced(type);
                isProducing = true;
                currentProgress = 0;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

long CommandCenter::getTimeToFinish()
{
    if (isProducing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

void CommandCenter::applyChronoBoost()
{
}

EntityType SupplyDepot::getType()
{
    return TERRAN_SUPPLY_DEPOT;
}

EntityType Barracks::getType()
{
    return TERRAN_BARRACKS;
}

void Barracks::update(GameState& state)
{
    if (isProducing)
    {
        currentProgress ++;
        if (currentProgress >= maxProgress)
        {
            state.addEntity(TERRAN_MARINE, 1);
            isProducing = false;
        }
    }
}

bool Barracks::produceEntityIfPossible(EntityType type, GameState& state)
{
    switch(type){
        case EntityType::TERRAN_MARINE:
            if(state.hasEnough(50, 0, 1)){
                isProducing = true;
                state.consumeEnoughMinerals(50);
                state.notifyEntityIsBeingProduced(type);
                currentProgress = 0;
                return true;
            }
            break;
        default:
            return false;
    }
    return false;
}

void Barracks::applyChronoBoost()
{
}

long Barracks::getTimeToFinish()
{
    if (isProducing)
    {
        return maxProgress - currentProgress;
    }
    return 0;
}

EntityType Factory::getType()
{
    return TERRAN_FACTORY;
}

EntityType Refinery::getType()
{
    return TERRAN_REFINERY;
}

EntityType Hellion::getType()
{
    return TERRAN_HELLION;
}

EntityType Starport::getType()
{
    return TERRAN_STARPORT;
}

EntityType BattleCruiser::getType()
{
    return TERRAN_BATTLECRUISER;
}

Starport::Starport()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

Factory::Factory()
{
    interfaceBitmask = PRODUCER_INTERFACE | UPGRADABLE_INTERFACE | UPDATABLE_INTERFACE;
}

/* TODO: IMPLEMENT the functions below, as they are only dummies */

bool Starport::produceEntityIfPossible(EntityType type, GameState &state)
{
}

long Starport::getTimeToFinish()
{
    // TODO: Implement
    return Producer::getTimeToFinish();
}

void Starport::applyChronoBoost()
{
    // TODO: Implement
    Producer::applyChronoBoost();
}

bool Factory::produceEntityIfPossible(EntityType type, GameState &state)
{
}

void Factory::update(GameState &state)
{
    // TODO: Implement
    Updatable::update(state);
}

void Factory::applyChronoBoost()
{
    // TODO: Implement
    Producer::applyChronoBoost();
}

long Factory::getTimeToFinish()
{
    // TODO: Implement
    return Producer::getTimeToFinish();
}

bool Factory::isUpgradable(GameState& state, EntityType type){
    return false;
}
void Factory::upgrade(GameState& state, EntityType to){

}

bool Starport::isUpgradable(GameState& state, EntityType type){
    return false;
}
void Starport::upgrade(GameState& state, EntityType to){

}

SCV::SCV()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE | WORKER_INTERFACE;
}

CommandCenter::CommandCenter()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

Barracks::Barracks()
{
    interfaceBitmask = UPDATABLE_INTERFACE | PRODUCER_INTERFACE;
}

EntityType Armory::getType() {
	return TERRAN_ARMORY;
}

EntityType Banshee::getType() {
	return TERRAN_BANSHEE;
}

EntityType BarracksReactor::getType() {
	return TERRAN_BARRACKS_REACTOR;
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

EntityType BarracksTechLab::getType() {
	return TERRAN_BARRACKS_TECH_LAB;
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

