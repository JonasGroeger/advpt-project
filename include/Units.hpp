#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Producer.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class SCV : public Entity, 
            public Worker,
            public Updatable,
            public Producer
{
    /* Should be refactored int common class */
    private:
        int currentProgress = 0;
        int maxProgress = 0;
        EntityType product = NONE;

    /* class Entity */
    public:
        SCV();
        ;

    /* class Updatable */
    public:
        virtual void update(GameState& state);

    /* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state);
        virtual void applyChronoBoost();
        virtual long getTimeToFinish();
};

class SupplyDepot: public Entity
{
    /* class Entity */
    public:
        SupplyDepot(){
            type = TERRAN_SUPPLY_DEPOT;
        }
};

class Marine: public Entity
{
    public:
        Marine(){
            type = TERRAN_MARINE;
        }
};


class Refinery : public Entity
{
    public:
        Refinery(){
            type = TERRAN_REFINERY;
        }
};

class Hellion : public Entity
{
    /* class Entity */
    public:
        Hellion(){
            type = TERRAN_HELLION;
        }
};

class BattleCruiser : public Entity
{
    /* class Entity */
    public:
        BattleCruiser(){
            type = TERRAN_BATTLECRUISER;
        }
};

class Armory : public Entity
{
	public:
        Armory(){
            type = TERRAN_ARMORY;
        }
};

class Banshee : public Entity
{
	public:
        Banshee(){
            type = TERRAN_BANSHEE;
        }
};

class Bunker : public Entity
{
	public:
        Bunker(){
            type = TERRAN_BUNKER;
        }
};


class Reaper : public Entity
{
	public:
        Reaper(){
            type = TERRAN_REAPER;
        }
};

class Ghost : public Entity
{
	public:
        Ghost(){
            type = TERRAN_GHOST;
        }

};

class Raven: public Entity
{
	public:
        Raven(){
            type = TERRAN_RAVEN;
        }

};

class FusionCore : public Entity
{
	public:
        FusionCore(){
            type = TERRAN_FUSION_CORE;
        }

};

class Viking : public Entity
{
	public:
        Viking(){
            type = TERRAN_VIKING;
        }

};

class Medivac : public Entity
{
	public:
        Medivac(){
            type = TERRAN_MEDIVAC;
        }

};

class SiegeTank : public Entity
{
	public:
        SiegeTank(){
            type = TERRAN_SIEGE_TANK;
        }

};

class Thor : public Entity
{
	public:
        Thor(){
            type = TERRAN_THOR;
        }

};

class OrbitalCommand : public Entity
{
	public:
        OrbitalCommand(){
            type = TERRAN_ORBITAL_COMMAND;
        }

};

class EngineeringBay : public Entity
{
	public:
        EngineeringBay(){
            type = TERRAN_ENGINEERING_BAY;
        }

};

class PlanetaryFortress : public Entity,
                          public Producer
{
	public:
        PlanetaryFortress(){
            interfaceBitmask = PRODUCER_INTERFACE | UPDATABLE_INTERFACE;
            type = TERRAN_PLANETARY_FORTRESS;
        }

		/* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state);
        virtual long getTimeToFinish() override;
        virtual void applyChronoBoost() override;
	
};

class MissileTurret : public Entity
{
	public:
        MissileTurret(){
            type = TERRAN_MISSILE_TURRET;
        }

};

class SensorTower : public Entity
{
	public:
        SensorTower(){
            type = TERRAN_SENSOR_TOWER;
        }

};

class GhostAcademy : public Entity
{
	public:
        GhostAcademy(){
            type = TERRAN_GHOST_ACADEMY;
        }

};

