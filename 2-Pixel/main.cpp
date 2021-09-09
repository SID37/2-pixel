#include "world.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <iostream>
#include <vector>


int main()
{
	int width = 1920 / 3;
	int height = 1080 / 3;
	sf::RenderWindow window(sf::VideoMode(width, height), "2-pixel");

	World world(width, height);
	Player& player = world.addPlayer();
	sf::Clock clock;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			float kx = width / (float)window.getSize().x;
			float ky = height / (float)window.getSize().y;
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				player.action(event.key.code, true);
				break;
			case sf::Event::KeyReleased:
				player.action(event.key.code, false);
				break;
			case sf::Event::MouseButtonPressed:
				player.action(event.mouseButton.button, true);
				player.mouseMove(event.mouseButton.x * kx, event.mouseButton.y * ky);
				break;
			case sf::Event::MouseButtonReleased:
				player.action(event.mouseButton.button, false);
				player.mouseMove(event.mouseButton.x * kx, event.mouseButton.y * ky);
				break;
			case sf::Event::MouseMoved:
				player.mouseMove(event.mouseMove.x * kx, event.mouseMove.y * ky);
				break;
			}
		}

		float dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		world.tick(dt);
		
		window.clear({0, 0, 0});
		window.draw(world);
		window.display();
	}
}
