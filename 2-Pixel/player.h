#pragma once

#include "entity.h"
#include "canvas.h"
#include "world.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

class Player: public Entity
{
	friend class World;
	friend class Fireball;

protected:
	World& world;
	Canvas canvas;
	Canvas pointer;
	float speed;
	float _x;
	float _y;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	Player(World& world);

	enum Action {
		ACTION_FIREBALL,
		ACTION_FLAMETHROWER,
	};

	struct {
		int hp;
		int direction = 0;
		bool canJump = true;
		int mouse_x = 0;
		int mouse_y = 0;
		bool l_activate = false;
		bool r_activate = false;
		bool shift = false;
		float fireball_time = 0;
		float flamethrower_time = 0;
		Action action = ACTION_FIREBALL;
	} state;

	void action(float dt);
	void actionSet(float dt);
	void actionRemove(float dt);
	void actionFireball(float dt);
	void actionFlamethrower(float dt);


	void respawn();
	bool canMove(float x, float y) const;
	float trace(int x, int y, float dx, float dy) const;
	void relocatePointer();

public:
	int X() const;
	int Y() const;

	void move(float x, float y);
	void action(sf::Keyboard::Key key, bool down);
	void action(sf::Mouse::Button key, bool down);
	void mouseMove(int x, int y);
	virtual void tick(float dt) override;
};
