#pragma once

#include "Entity.hpp"
#include "EntityType.hpp"
#include "entities/Producer.hpp"
#include "Upgradable.hpp"
#include "Updatable.hpp"
#include "GameState.hpp"
#include "entities/Worker.hpp"

////////////////////////////////////////////
//      v The green protoss units. v      //
////////////////////////////////////////////

class Zealot : public Entity
{
    /* class Entity */
public:
    Zealot()
    {
        type = PROTOSS_ZEALOT;
    }
};

class Stalker : public Entity
{
    /* class Entity */
public:
    Stalker()
    {
        type = PROTOSS_STALKER;
    }
};

class Sentry : public Entity
{
    /* class Entity */
public:
    Sentry()
    {
        type = PROTOSS_SENTRY;
    }
};

class WarpPrism : public Entity
{
    /* class Entity */
public:
    WarpPrism()
    {
        type = PROTOSS_WARP_PRISM;
    }
};

class Immortal : public Entity
{
    /* class Entity */
public:
    Immortal()
    {
        type = PROTOSS_IMMORTAL;
    }
};

class Observer : public Entity
{
    /* class Entity */
public:
    Observer()
    {
        type = PROTOSS_OBSERVER;
    }
};

class Colossus : public Entity
{
    /* class Entity */
public:
    Colossus()
    {
        type = PROTOSS_COLOSSUS;
    }
};

class HighTemplar : public Entity
{
    /* class Entity */
public:
    HighTemplar()
    {
        type = PROTOSS_HIGH_TEMPLAR;
    }
};

class DarkTemplar : public Entity
{
    /* class Entity */
public:
    DarkTemplar()
    {
        type = PROTOSS_DARK_TEMPLAR;
    }
};

class Archon : public Entity
{
    /* class Entity */
public:
    Archon()
    {
        type = PROTOSS_ARCHON;
    }
};

class MotherShip : public Entity
{
    /* class Entity */
public:
    MotherShip()
    {
        type = PROTOSS_MOTHERSHIP;
    }
};

class Phoenix : public Entity
{
    /* class Entity */
public:
    Phoenix()
    {
        type = PROTOSS_PHOENIX;
    }
};

class VoidRay : public Entity
{
    /* class Entity */
public:
    VoidRay()
    {
        type = PROTOSS_VOID_RAY;
    }
};

class Carrier : public Entity
{
    /* class Entity */
public:
    Carrier()
    {
        type = PROTOSS_CARRIER;
    }
};

////////////////////////////////////////////
//     v The blue protoss buildings v     //
////////////////////////////////////////////

class Assimilator : public Entity
{
    /* class Entity */
public:
    Assimilator()
    {
        type = PROTOSS_ASSIMILATOR;
    }
};

class Forge : public Entity
{
    /* class Entity */
public:
    Forge()
    {
        type = PROTOSS_FORGE;
    }
};

class PhotonCannon : public Entity
{
    /* class Entity */
public:
    PhotonCannon()
    {
        type = PROTOSS_PHOTON_CANNON;
    }
};

class Pylon : public Entity
{
    /* class Entity */
public:
    Pylon()
    {
        type = PROTOSS_PYLON;
    }
};

class CyberneticsCore : public Entity
{
    /* class Entity */
public:
    CyberneticsCore()
    {
        type = PROTOSS_CYBERNETICS_CORE;
    }
};

class RoboticsBay : public Entity
{
    /* class Entity */
public:
    RoboticsBay()
    {
        type = PROTOSS_ROBOTICS_BAY;
    }
};

class TwilightCouncil : public Entity
{
    /* class Entity */
public:
    TwilightCouncil()
    {
        type = PROTOSS_TWILIGHT_COUNCIL;
    }
};

class TemplarArchives : public Entity
{
    /* class Entity */
public:
    TemplarArchives()
    {
        type = PROTOSS_TEMPLAR_ARCHIVES;
    }
};

class DarkShrine : public Entity
{
    /* class Entity */
public:
    DarkShrine()
    {
        type = PROTOSS_DARK_SHRINE;
    }
};

class FleetBeacon : public Entity
{
    /* class Entity */
public:
    FleetBeacon()
    {
        type = PROTOSS_FLEET_BEACON;
    }
};

