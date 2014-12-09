#pragma once

class Entity {
	public:
		EntityType type;
		virtual EntityType getType() = 0;
	
}
