#pragma once

class Constructable
{
public:
    virtual bool isConstrucable(GameState state);
    virtual void construct(GameState state);
    virtual ~Constructable();
};
