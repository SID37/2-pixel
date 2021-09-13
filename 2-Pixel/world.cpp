#include "world.h"
#include <stdlib.h>

void World::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(environment, states);
	for (const Entity* e: entityes)
		target.draw(*e, states);
}

static int rand(int from, int to)
{
	int delta = to - from;
	if (delta <= 0)
		return from;
	return rand() % delta + from;
}

void World::drawLine(sf::Color colorFrom, sf::Color colorTo, float reliability, int height, int range)
{
	int width = environment.Width();
	int shift = 1;
	while (width > shift) shift <<= 1;

	std::vector<int> buf(shift << 1);
	buf[1] = buf[0] = height;
	for (int i = 2; i < (shift << 1); i <<= 1)
	{
		for (int j = i; j < (i << 1); ++j)
		{
			if (j & 1)
			{
				int from = buf[j >> 1];
				int to = buf[(j >> 1) + 1];
				buf[j] = rand(std::min(from, to) - range, std::max(from, to) + range);
			} else
				buf[j] = buf[j >> 1];
		}
		range >>= 1;
	}
	
	for (int i = 0; i < width; ++i)
		for (int j = buf[buf.size() - i - 1]; j < environment.Height(); ++j)
			environment[i][j] = Environment::Pixel::Random(
				{ colorFrom, reliability },
				{ colorTo, reliability });
}

World::World(int width, int height):
	environment(width, height)
{
	drawLine({ 0, 100, 0 }, { 0, 150, 0 }, 1, height / 2, height / 10);
	drawLine({ 100, 50, 0 }, { 120, 70, 20 }, 2, height / 2 + height / 5, height / 10);
	drawLine({ 100, 100, 100 }, { 120, 120, 120 }, 3, height / 2 + height / 3, height / 10);
}

void World::tick(float dt)
{
	if (dt > 0.1)
		dt = 0.1;
	for (int i = 0; i < entityes.size(); ++i)
		entityes[i]->tick(dt);
}


Fireball& World::addFireball(float x, float y, float dx, float dy)
{
	Fireball* p = new Fireball(*this, x, y, dx, dy, Fireball::FIREBALL);
	entityes.push_back(p);
	return *p;
}

Fireball& World::addFlamethrower(float x, float y, float dx, float dy)
{
	Fireball* p = new Fireball(*this, x, y, dx, dy, Fireball::FLAMETHROWER);
	entityes.push_back(p);
	return *p;
}

Player& World::addPlayer()
{
	Player* p = new Player(*this);
	entityes.push_back(p);
	players.push_back(p);
	return *p;
}

World::~World()
{
	for (Entity* p : entityes)
		delete p;
}
