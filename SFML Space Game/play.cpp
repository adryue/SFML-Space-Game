#include "play.h"
#include "spaceship.h"

ScreenName playScreen(sf::RenderWindow& window)
{
	Spaceship ship1;

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				return ScreenName::none;
			}
		}
		window.clear();

		ship1.handleInputs();
		ship1.update();

		ship1.draw(window);

		window.display();
	}

	return ScreenName::none;
}