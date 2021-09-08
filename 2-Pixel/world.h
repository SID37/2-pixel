#pragma once

#include "environment.h"
//#include "player.h"

#include <SFML/Graphics/Color.hpp>


class World: public sf::Drawable
{
	Environment environment;
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void drawLine(sf::Color colorFrom, sf::Color colorTo, float reliability, int height, int range);

public:
	World(int width, int height);

};
