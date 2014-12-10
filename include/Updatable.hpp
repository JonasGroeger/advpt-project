#pragma once

class Updatable
{
public:
	virtual void update(GameState state);
	virtual ~Updatable();
};
