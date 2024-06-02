#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"

const int FRAMERATE = 60;
const int WIN_X_LEN = 1280;
const int WIN_Y_LEN = 720;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIN_X_LEN, WIN_Y_LEN), "Space Game");
	window.setFramerateLimit(FRAMERATE);
	
	ScreenName nextScreen = ScreenName::play;

	while (nextScreen != ScreenName::none)
	{
		switch (nextScreen)
		{
		case ScreenName::home:
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