#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "home.h"

const int FRAMERATE = 60;
int WIN_X_LEN = 1280;
int WIN_Y_LEN = 720;
sf::Font FONT;

int main()
{
	FONT.loadFromFile("Assets/CourierPrime-Regular.ttf");

	sf::RenderWindow window(sf::VideoMode(WIN_X_LEN, WIN_Y_LEN), "Space Game");
	window.setFramerateLimit(FRAMERATE);
	
	ScreenName nextScreen = ScreenName::home;

	while (nextScreen != ScreenName::none)
	{
		window.setView(window.getDefaultView());

		switch (nextScreen)
		{
		case ScreenName::home:
			nextScreen = homeScreen(window);
			break;
		case ScreenName::play:
			nextScreen = playScreen(window);
			break;
		case ScreenName::none:
			break;
		}
	}

	/*
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		
		window.display();
	}
	*/
}