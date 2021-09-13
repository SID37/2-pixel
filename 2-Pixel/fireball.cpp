#include "feireball.h"

#include <cmath>


template <typename T>
static T Sqr(T x)
{
	return x * x;
}

float rand01()
{
	return rand() / (float) RAND_MAX;
}

void Fireball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (ttl <= 0) return;
	target.draw(canvas, states);
}


Fireball::Fireball(World& world, float x, float y, float x_speed, float y_speed, Type type):
	world(world),
	canvas(30, 30, x - 15, y - 15),
	_x(x - 15), _y(y - 15),
	x_speed(x_speed), y_speed(y_speed),
	type(type)
{
	switch (type)
	{
	case Fireball::FIREBALL:
		ttl = 10;
		damage = 0.01;
		break;
	case Fireball::FLAMETHROWER:
		ttl = 1;
		damage = 0.01;
		break;
	}
}

void Fireball::move(float x, float y)
{
	_x = x;
	_y = y;
	canvas.Move(x, y);
}

int Fireball::X() const
{
	return _x;
}

int Fireball::Y() const
{
	return _y;
}

void Fireball::tick(float dt)
{
	ttl -= dt;
	if (ttl <= 0) return;

	move(_x + x_speed * dt, _y + y_speed * dt);

	for (int i = 0; i < canvas.Width(); ++i)
		for (int j = 0; j < canvas.Height(); ++j)
		{
			float len = std::sqrt(Sqr(canvas.Width() / 2 - i) +
				Sqr(canvas.Height() / 2 - j));
			float appr = std::min((canvas.Width() - 2 * len) / canvas.Width(), 1.0f);
			if (appr > rand01())
			{
				if (type == FIREBALL)
					canvas.SetPixel(i, j, Environment::Pixel::Random(
						{ { 0, 100, 0 }, 0 },
						{ { 100, 200, 100 }, 0 }
					).color);
				else if (type == FLAMETHROWER)
					canvas.SetPixel(i, j, Environment::Pixel::Random(
						{ { 150, 50, 0 }, 0 },
						{ { 200, 140, 10 }, 0 }
					).color);

				int global_x = i + canvas.X();
				int global_y = j + canvas.Y();
				if (global_x < 0 || global_x >= world.environment.Width() ||
					global_y < 0 || global_y >= world.environment.Height())
					continue;
				Environment::Pixel& pixel = world.environment[global_x][global_y];
				if (pixel.reliability > 0)
				{
					pixel.reliability -= 1;
					ttl -= damage;
					if (pixel.reliability < 1)
						pixel = { {0, 0, 0}, 0 };
				}
				for (Player* p : world.players)
				{
					if (p->X() <= global_x && p->X() + p->canvas.Width() > global_x &&
						p->Y() <= global_y && p->Y() + p->canvas.Height() > global_y)
					{
						int p_x = global_x - p->X();
						int p_y = global_y - p->Y();
						if (p->canvas.GetPixel(p_x, p_y).a > 0)
						{
							p->canvas.SetPixel(p_x, p_y, {0, 0, 0, 0});
							p->state.hp -= 1;
							ttl -= damage * 5;
						}
					}
				}
			}
			else
				canvas.SetPixel(i, j, { 0, 0, 0, 0 });
		}
}
