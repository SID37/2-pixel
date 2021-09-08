#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Image.hpp>


class Canvas: public sf::Drawable
{
	mutable sf::Texture texture;
	sf::Image image;
	sf::Sprite sprite;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Canvas(int width, int height, int x = 0, int y = 0);

	int Width() const;
	int Height() const;

	void Move(int x, int y);
	int X() const;
	int Y() const;

	void SetPixel(int x, int y, sf::Color color);
	sf::Color GetPixel(int x, int y) const;
};
