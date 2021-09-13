#pragma once


#include <SFML/Graphics/Drawable.hpp>

class Entity : public sf::Drawable
{

public:
	virtual void tick(float dt) = 0;

};
