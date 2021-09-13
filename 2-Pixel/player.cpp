#include "player.h"

#include <cmath>


template <typename T>
static T Sqr(T x)
{
	return x * x;
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(canvas, states);
	target.draw(pointer, states);
}


void Player::respawn()
{
	move(rand() % (world.environment.Width() - canvas.Width()), 0);
	state.hp = canvas.Width() * canvas.Height() / 2;
	for (int i = 0; i < canvas.Width(); ++i)
		for (int j = 0; j < canvas.Height(); ++j)
			canvas.SetPixel(i, j,
				Environment::Pixel::Random(
					{ { 15, 20, 150 }, 0 }, { { 20, 30, 160 }, 0 }).color);
}


Player::Player(World& world) :
	world(world),
	canvas(10, 20),
	speed(0),
	pointer(11, 11)
{
	for (int i = 0; i < pointer.Width(); ++i)
		for (int j = 0; j < pointer.Height(); ++j)
			pointer.SetPixel(i, j, { 0, 0, 0, 0 });
	for (int i = 0; i < pointer.Width(); ++i)
		pointer.SetPixel(i, pointer.Height() / 2, { 0, 200, 0 });
	for (int i = 0; i < pointer.Height(); ++i)
		pointer.SetPixel(pointer.Width() / 2, i, { 0, 200, 0 });
	respawn();
}

bool Player::canMove(float x, float y) const
{
	int int_x = x;
	int int_y = y;
	int counter = 0;
	for (int i = std::max(0, int_x); i < std::min(world.environment.Width(), int_x + canvas.Width()); ++i)
		for (int j = std::max(0, int_y); j < std::min(world.environment.Height(), int_y + canvas.Height()); ++j)
			if (world.environment[i][j].reliability > 0)
				++counter;
	return counter <= canvas.Width() * canvas.Height() / 50;
}

float Player::trace(int x, int y, float dx, float dy) const
{
	int border[]{ world.environment.Width(), world.environment.Height() };
	int pos[]{ x, y };
	int step[]{ dx > 0 ? 1 : -1, dy > 0 ? 1 : -1 };
	float len = std::sqrt(dx * dx + dy * dy);
	float vect[]{ std::abs(len / dx), std::abs(len / dy) };
	float next[]{ vect[0], vect[1] };

	while(true)
	{
		int idx = next[0] < next[1] ? 0 : 1;
		pos[idx] += step[idx];

		if (pos[idx] < 0 || pos[idx] >= border[idx] ||
			world.environment[pos[0]][pos[1]].reliability > 0 ||
			next[idx] >= len)
			return next[idx] / len;
		next[idx] += vect[idx];
	}
}

int Player::X() const
{
	return _x;
}

int Player::Y() const
{
	return _y;
}

void Player::move(float x, float y)
{
	_x = x;
	_y = y;
	canvas.Move(x, y);
}

void Player::action(sf::Keyboard::Key key, bool down)
{
	switch (key)
	{
	case sf::Keyboard::A:
		state.direction += down ? -1 : 1;
		break;
	case sf::Keyboard::D:
		state.direction += down ? 1 : -1;
		break;
	case sf::Keyboard::W:
	case sf::Keyboard::Space:
		if (down && state.canJump)
		{
			speed = -300;
			state.canJump = false;
		}
		break;
	case sf::Keyboard::Num1:
		state.action = ACTION_FIREBALL;
		break;
	case sf::Keyboard::Num2:
		state.action = ACTION_FLAMETHROWER;
		break;
	case sf::Keyboard::LShift:
		state.shift = down;
		break;
	}
	state.direction = std::min(state.direction, 1);
	state.direction = std::max(state.direction, -1);
}

void Player::action(sf::Mouse::Button key, bool down)
{
	if (key == sf::Mouse::Left)
		state.l_activate = down;
	if (key == sf::Mouse::Right)
		state.r_activate = down;
}

void Player::action(float dt)
{
	state.fireball_time -= dt;
	state.flamethrower_time -= dt;
	if (state.r_activate)
	{
		if (state.shift) actionRemove(dt);
		else actionSet(dt);
	}
	if (state.l_activate)
	{
		switch (state.action)
		{
		case ACTION_FIREBALL: actionFireball(dt); break;
		case ACTION_FLAMETHROWER: actionFlamethrower(dt); break;
		}
	}
}

void Player::actionSet(float dt)
{
	if (Sqr(pointer.X() + pointer.Width() / 2 - state.mouse_x) +
		Sqr(pointer.Y() + pointer.Height() / 2 - state.mouse_y) > 10 ||
		Sqr(canvas.X() + canvas.Width() / 2 - pointer.X() - pointer.Width() / 2) +
		Sqr(canvas.Y() + canvas.Height() / 2 - pointer.Y() - pointer.Height() / 2) <=
		Sqr(canvas.Width() / 2 + pointer.Width() / 2) +
		Sqr(canvas.Height() / 2 + pointer.Height() / 2))
		return;

	int delta = 5;
	int x_from = std::max(0, state.mouse_x - delta);
	int x_to = std::min(world.environment.Width(), state.mouse_x + pointer.Width() + delta);
	int y_from = std::max(0, state.mouse_y - delta);
	int y_to = std::min(world.environment.Height(), state.mouse_y + pointer.Height() + delta);
	int x_m = (x_from + x_to) / 2;
	int y_m = (y_from + y_to) / 2;

	for (int i = x_from; i < x_to; ++i)
		for (int j = y_from; j < y_to; ++j)
			if (world.environment[i][j].reliability == 0 &&
				Sqr(i - x_m) + Sqr(j - y_m) <= Sqr(pointer.Width() / 2 + delta))
				world.environment[i][j] = Environment::Pixel::Random(
					{ { 100, 50, 0 }, 2 }, { { 120, 70, 20 }, 2 });
}

void Player::actionRemove(float dt)
{
	int delta = 7;
	int x_from = std::max(0, pointer.X() - delta);
	int x_to = std::min(world.environment.Width(), pointer.X() + pointer.Width() + delta);
	int y_from = std::max(0, pointer.Y() - delta);
	int y_to = std::min(world.environment.Height(), pointer.Y() + pointer.Height() + delta);
	int x_m = (x_from + x_to) / 2;
	int y_m = (y_from + y_to) / 2;

	for (int i = x_from; i < x_to; ++i)
		for (int j = y_from; j < y_to; ++j)
			if (Sqr(i - x_m) + Sqr(j - y_m) <= Sqr(pointer.Width() / 2 + delta))
				if (world.environment[i][j].reliability > 1)
					world.environment[i][j].reliability -= 0.3;
				else
					world.environment[i][j] = { {0, 0, 0}, 0 };
}

void Player::actionFireball(float dt)
{
	if (state.fireball_time > 0)
	{
		return;
	}

	float center_x = (canvas.X() + canvas.Width() / 2);
	float center_y = (canvas.Y() + canvas.Height() / 2);

	float dx = state.mouse_x - center_x;
	float dy = state.mouse_y - center_y;
	float len = std::sqrt(dx * dx + dy * dy);
	float speed = 300;

	world.addFireball(
		center_x + 30 * dx / len,
		center_y + 30 * dy / len,
		speed * dx / len,
		speed * dy / len);
	state.fireball_time = 0.2;
}

void Player::actionFlamethrower(float dt)
{
	if (state.flamethrower_time > 0)
	{
		state.flamethrower_time -= dt;
		return;
	}

	float center_x = (canvas.X() + canvas.Width() / 2);
	float center_y = (canvas.Y() + canvas.Height() / 2);

	float dx = state.mouse_x - center_x;
	float dy = state.mouse_y - center_y;
	float len = std::sqrt(dx * dx + dy * dy);
	float speed = 100;

	world.addFlamethrower(
		center_x + 30 * dx / len,
		center_y + 30 * dy / len,
		speed * dx / len,
		speed * dy / len);
	state.flamethrower_time = 0;
}

void Player::relocatePointer()
{
	int pos[]{
		X() + canvas.Width() / 2,
		Y() + canvas.Height() / 2,
	};
	float vect[]{
		state.mouse_x - pos[0],
		state.mouse_y - pos[1],
	};
	float k = trace(pos[0], pos[1], vect[0], vect[1]);
	pointer.Move(
		pos[0] + vect[0] * k - pointer.Width() / 2,
		pos[1] + vect[1] * k - pointer.Height() / 2);
}

void Player::mouseMove(int x, int y)
{
	state.mouse_x = x;
	state.mouse_y = y;
}


void Player::tick(float dt)
{
	if (state.hp <= 0)
		respawn();
	relocatePointer();
	action(dt);

	bool moved = false;
	if (canMove(_x + dt * state.direction * 100, _y))
	{
		move(_x + dt * state.direction * 100, _y);
		moved = true;
	}

	speed += dt * 1000;
	int add = 0;
	if (!canMove(_x, _y + speed * dt))
	{
		speed = state.direction != 0 && moved ? -50 : 0;
		state.canJump = true;
	}
	move(_x, _y + speed * dt + add);
}
