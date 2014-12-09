#pragma once

class Producer {
	public:
		virtual ~Producer();
		virtual bool canProduce(EntityType type, GameState state);
		virtual void produce(EntityType type, GameState state);
		virtual void applyChronoBoost();
		
	
}
