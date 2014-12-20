#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "Producer.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

class Updatable;

class SCV : public Entity, 
            public Worker,
            public Updatable,
            public Producer
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        int maxProgress = 0;
        EntityType product = NONE;

    /* class Entity */
    public:
        SCV();
        virtual EntityType getType();

    /* class Updatable */
    public:
        virtual void update(GameState& state);

    /* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state);
        virtual void applyChronoBoost();
        virtual long getTimeToFinish();
};

class CommandCenter: public Entity,
                     public Producer,
                     public Updatable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 17; // Build time of scv

    /* class Entity */
    public:
        CommandCenter();
        virtual EntityType getType();

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
        virtual EntityType getType();
};

class Barracks: public Entity,
                public Producer,
                public Updatable
{
    /* Should be refactored int common class */
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 25; // Build time of a marine

    /* class Entity */
    public:
        Barracks();
        virtual EntityType getType();

    /* class Updatable */
    public:
        virtual void update(GameState& state);

    /* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState& state);
        virtual void applyChronoBoost();
        virtual long getTimeToFinish();
};

class Refinery : public Entity
{
    public:
        virtual EntityType getType() override;
};

class Factory : public Entity,
                public Producer,
                public Updatable,
                public Upgradable
{
    private:
        bool isProducing = false;
        int currentProgress = 0;
        const int maxProgress = 30; // Build time of hellion

    /* class Entity */
    public:
        Factory();
        virtual EntityType getType() override;

    /* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state);
        virtual long getTimeToFinish() override;
        virtual void applyChronoBoost() override;

    /* class Updatable */
    public:
        virtual void update(GameState &state) override;

    /* class Upgradable */
    public:
        virtual bool isUpgradable(GameState& state, EntityType type);
        virtual void upgrade(GameState& state, EntityType to);
};

class Hellion : public Entity
{
    /* class Entity */
    public:
        virtual EntityType getType() override;
};

class Starport : public Entity,
                 public Producer,
                 public Updatable,
                 public Upgradable
{
    /* class Entity */
    public:
        Starport();
        virtual EntityType getType() override;


    virtual bool produceEntityIfPossible(EntityType type, GameState &state) override;

    virtual long getTimeToFinish() override;

    virtual void applyChronoBoost() override;

    /* class Upgradable */
    public:
        virtual bool isUpgradable(GameState& state, EntityType type);
        virtual void upgrade(GameState& state, EntityType to);
};

class BattleCruiser : public Entity
{
    /* class Entity */
    public:
        virtual EntityType getType() override;
};

class Armory : public Entity
{
	public:
		virtual EntityType getType() override;
};

class Banshee : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Bunker : public Entity
{
	public:
		virtual EntityType getType() override;
	
};


class Reaper : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Ghost : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Raven: public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class FusionCore : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Viking : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Medivac : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class SiegeTank : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class Thor : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class OrbitalCommand : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class EngineeringBay : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class PlanetaryFortress : public Entity,
                          public Producer
{
	public:
		virtual EntityType getType() override;
		
		/* class Producer */
    public:
        virtual bool produceEntityIfPossible(EntityType type, GameState &state);
        virtual long getTimeToFinish() override;
        virtual void applyChronoBoost() override;
	
};

class MissileTurret : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class SensorTower : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class BarracksReactor : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class BarracksTechLab : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class GhostAcademy : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class FactoryTechLab : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class FactoryReactor : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class StarportTechLab : public Entity
{
	public:
		virtual EntityType getType() override;
	
};

class StarportReactor : public Entity
{
	public:
		virtual EntityType getType() override;
	
};
