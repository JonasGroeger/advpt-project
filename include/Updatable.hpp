#pragma once

//#include "GameState.hpp"

// Forward declaration of GameState so this compiles
class GameState;

class Updatable
{
public:
	virtual void update(GameState& state);
	virtual ~Updatable();
};
