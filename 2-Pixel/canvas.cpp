#include "canvas.h"


void Canvas::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	texture.update(image);
	target.draw(sprite, states);
}

Canvas::Canvas(int width, int height, int x, int y)
{
	image.create(width, height);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	Move(x, y);
}

void Canvas::SetPixel(int x, int y, sf::Color color)
{
	image.setPixel(x, y, color);
}

sf::Color Canvas::GetPixel(int x, int y) const
{
	return image.getPixel(x, y);
}

int Canvas::Width() const
{
	return image.getSize().x;
}

void Canvas::Move(int x, int y)
{
	sprite.setPosition(x, y);
}

int Canvas::X() const
{
	return sprite.getPosition().x;
}

int Canvas::Y() const
{
	return sprite.getPosition().y;
}

int Canvas::Height() const
{
	return image.getSize().y;
}
