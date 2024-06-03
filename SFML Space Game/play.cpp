#include <iostream>
#include <vector>
#include "play.h"


std::vector<Bullet> bullets;

void addBullet(Bullet bullet) //used inside the spaceship class
{
	bullets.push_back(bullet);
}

ScreenName playScreen(sf::RenderWindow& window)
{
	Spaceship ship0(0);
	Spaceship ship1(1);

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

		//ship inputs
		ship0.handleInputs();
		ship1.handleInputs();

		//update ship positions
		ship0.update();
		ship1.update();

		//update bullets
		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i].update();
			if (ship0.handleCollision(bullets[i]) || ship1.handleCollision(bullets[i]))
			{
				bullets.erase(bullets.begin() + i);
				i--;
			}
		}

		//std::cout << sf::Joystick::getAxisPosition(1, sf::Joystick::U) << " " << sf::Joystick::getAxisPosition(1, sf::Joystick::V) << std::endl;

		//draw everything
		ship0.draw(window);
		ship1.draw(window);
		for (Bullet& b : bullets)
		{
			b.draw(window);
		}
		window.display();
	}

	return ScreenName::none;
}