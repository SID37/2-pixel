#include "environment.h"

void Environment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < canvas.Width(); ++i)
		for (int j = 0; j < canvas.Height(); ++j)
			canvas.SetPixel(i, j, data[i][j].color);
	target.draw(canvas, states);
}

Environment::Environment(int width, int height):
	data(width, std::vector<Pixel>(height, { {0, 0, 0, 0}, 0 })),
	canvas(width, height)
{ }

int Environment::Width() const
{
	return canvas.Width();
}

int Environment::Height() const
{
	return canvas.Height();
}

std::vector<Environment::Pixel>& Environment::operator[](int x)
{
	return data[x];
}

const std::vector<Environment::Pixel>& Environment::operator[](int x) const
{
	return data[x];
}
