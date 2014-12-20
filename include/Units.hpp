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
        ;
};

class Refinery : public Entity
{
    public:
};

class Hellion : public Entity
{
    /* class Entity */
    public:
};

class BattleCruiser : public Entity
{
    /* class Entity */
    public:
};

class Armory : public Entity
{
	public:
};

class Banshee : public Entity
{
	public:

};

class Bunker : public Entity
{
	public:

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

};

class Raven: public Entity
{
	public:

};

class FusionCore : public Entity
{
	public:

};

class Viking : public Entity
{
	public:

};

class Medivac : public Entity
{
	public:

};

class SiegeTank : public Entity
{
	public:

};

class Thor : public Entity
{
	public:

};

class OrbitalCommand : public Entity
{
	public:

};

class EngineeringBay : public Entity
{
	public:

};

class PlanetaryFortress : public Entity,
                          public Producer
{
	public:

		/* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state);
        virtual long getTimeToFinish() override;
        virtual void applyChronoBoost() override;
	
};

class MissileTurret : public Entity
{
	public:

};

class SensorTower : public Entity
{
	public:

};

class GhostAcademy : public Entity
{
	public:

};

class FactoryTechLab : public Entity
{
	public:

};

class FactoryReactor : public Entity
{
	public:

};

class StarportTechLab : public Entity
{
	public:

};

class StarportReactor : public Entity
{
	public:

};
