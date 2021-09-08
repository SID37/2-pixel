#pragma once

#include "canvas.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Color.hpp>
#include <vector>


class Environment: public sf::Drawable
{
public:
	struct Pixel
	{
		sf::Color color;
		float reliability;
	};

private:
	std::vector<std::vector<Pixel>> data;
	mutable Canvas canvas;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Environment(int width, int height);
	
	int Width() const;
	int Height() const;

	std::vector<Pixel>& operator[](int x);
	const std::vector<Pixel>& operator[](int x) const;
};
