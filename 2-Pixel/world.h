#pragma once

#include "environment.h"
#include "player.h"

#include <SFML/Graphics/Color.hpp>


class World: public sf::Drawable
{
	friend class Player;

	Environment environment;
	std::vector<Player*> players;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	
	void drawLine(sf::Color colorFrom, sf::Color colorTo, float reliability, int height, int range);

	World(const World&) = delete;
	World(World&&) = delete;
	World& operator=(const World&) = delete;
	World& operator=(World&&) = delete;
	
public:
	World(int width, int height);
	
	void tick(float dt);
	Player& addPlayer();
	virtual ~World();
};
