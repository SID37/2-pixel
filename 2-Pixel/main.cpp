#include "world.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>
#include <vector>


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920 / 3, 1080 / 3), "Render window");

	World world(1920 / 3, 1080 / 3);
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		std::cout << clock.getElapsedTime().asMilliseconds() << std::endl;

		window.clear({0, 0, 0});
		window.draw(world);
		window.display();
	}
}
