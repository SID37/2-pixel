#pragma once

#include "entity.h"
#include "canvas.h"
#include "world.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class Fireball : public Entity
{
	friend class World;

protected:
	enum Type
	{
		FIREBALL,
		FLAMETHROWER,
	};

	
	World& world;
	Canvas canvas;
	float x_speed;
	float y_speed;
	
	Type type;

	float ttl;
	float damage;

	float _x;
	float _y;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Fireball(World& world, float x, float y, float x_speed, float y_speed, Type type);
	void move(float x, float y);

public:
	int X() const;
	int Y() const;

	virtual void tick(float dt) override;
};
