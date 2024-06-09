#include <iostream>
#include <vector>
#include "play.h"

std::vector<Bullet> bullets;
std::vector<Laser> lasers;

void addBullet(Bullet bullet) //used inside the spaceship class
{
	bullets.push_back(bullet);
}

void addLaser(Laser laser) //used inside the spaceship class
{
	lasers.push_back(laser);
}

ScreenName playScreen(sf::RenderWindow& window)
{
	/*sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Images/background.png");
	backgroundTexture.setRepeated(true);
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, 100000, 100000));*/

	std::vector<Background> backgrounds;
	//note: you have to create the background variable before putting it into the vector
	//because you need to keep the instance of the texture
	Background bg0(0, 0.9, 1.0);
	Background bg1(1, 0.5, 0.5);
	Background bg2(2, 0.2, 0.5);
	backgrounds.push_back(bg0); 
	//backgrounds.push_back(bg1); 
	//backgrounds.push_back(bg2);
	//backgrounds.push_back(Background(1, 0.0));
	//backgrounds.push_back(Background(2, 1.0));

	Spaceship ship0(0);
	Spaceship ship1(1);

	Camera camera(2); //2 for the number of spaceships

	//TODO: add asteroids
	
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
		
		//---ship inputs---
		ship0.handleInputs();
		ship1.handleInputs();

		//---update ship positions---
		ship0.update();
		ship1.update();

		//---update bullets---
		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i].update())
			{
				bullets.erase(bullets.begin() + i);
				i--;
				continue;
			}
			if (ship0.handleCollision(bullets[i]) || ship1.handleCollision(bullets[i]))
			{
				bullets.erase(bullets.begin() + i);
				i--;
			}
		}

		//---update lasers---
		for (int i = 0; i < lasers.size(); i++)
		{
			if (lasers[i].update())
			{
				lasers.erase(lasers.begin() + i);
				i--;
				continue;
			}
			ship0.handleCollision(lasers[i]);
			ship1.handleCollision(lasers[i]);
		}

		//---draw everything---
		//update the camera view
		camera.coordinates[0] = ship0.position;
		camera.coordinates[1] = ship1.position;
		camera.updateView();
		window.setView(camera.view);
		//draw backgrounds
		//window.draw(background);
		for (Background& b : backgrounds)
		{
			b.update(camera.view.getCenter());
			b.draw(window);
		}
		//draw ships
		ship0.draw(window);
		ship1.draw(window);
		//draw bullets
		for (Bullet& b : bullets)
		{
			b.draw(window);
		}
		//draw lasers
		for (Laser& l : lasers)
		{
			l.draw(window);
		}
		//draw ui
		window.setView(window.getDefaultView());
		ship0.drawUI(window);
		ship1.drawUI(window);

		window.display();
	}

	return ScreenName::none;
}