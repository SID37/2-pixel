#pragma once

#include "entity.h"
#include "environment.h"
#include "player.h"
#include "feireball.h"

#include <SFML/Graphics/Color.hpp>


class World: public sf::Drawable
{
	friend class Player;
	friend class Fireball;

	Environment environment;
	std::vector<Entity*> entityes;
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
	Fireball& addFireball(float x, float y, float dx, float dy);
	Fireball& addFlamethrower(float x, float y, float dx, float dy);
	virtual ~World();
};
