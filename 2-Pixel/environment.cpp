#include "environment.h"


static int rand(int from, int to)
{
	int delta = to - from;
	if (delta <= 0)
		return from;
	return rand() % delta + from;
}

void Environment::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (int i = 0; i < canvas.Width(); ++i)
		for (int j = 0; j < canvas.Height(); ++j)
			canvas.SetPixel(i, j, data[i][j].color);
	target.draw(canvas, states);
}

Environment::Pixel Environment::Pixel::Random(const Pixel& from, const Pixel& to)
{
	return Pixel{ {
				(sf::Uint8)rand(from.color.r, to.color.r),
				(sf::Uint8)rand(from.color.g, to.color.g),
				(sf::Uint8)rand(from.color.b, to.color.b),
			}, rand(from.reliability * 100, to.reliability * 100) / (float)100
	};
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
