#include <iostream>
#include <vector>
#include "Camera.h"
#include "Background.h"
#include "spaceship.h"
#include "Ship0.h"
#include "Ship1.h"
#include "AsteroidSpawner.h"
#include "Text.h"
#include "play.h"

std::vector<Bullet> bullets;
std::vector<Laser> lasers;
std::vector<Asteroid> asteroids;

Camera camera(2);

void addBullet(Bullet bullet) //used inside the spaceship class
{
	bullets.push_back(bullet);
}

void addLaser(Laser laser) //used inside the spaceship class
{
	lasers.push_back(laser);
}

void addAsteroid(Asteroid asteroid) //used inside the Asteroid Spawner class
{
	asteroids.push_back(asteroid);
}

void addCameraShake(float amount)
{
	camera.addShake(amount);
}

ScreenName playScreen(sf::RenderWindow& window)
{
	//clear all objects from previous rounds
	bullets.clear();
	lasers.clear();
	asteroids.clear();

	/*sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Assets/Images/Background 0.png");
	backgroundTexture.setRepeated(true);
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, 100000, 100000));*/

	std::vector<Background> backgrounds;
	//note: you have to create the background variable before putting it into the vector
	//because you need to keep the instance of the texture
	Background bg0(5, 0.9, 1.0);
	Background bg1(1, 0.5, 0.5);
	Background bg2(2, 0.2, 0.5);
	backgrounds.push_back(bg0); 
	backgrounds.push_back(bg1); 
	backgrounds.push_back(bg2);
	//backgrounds.push_back(Background(1, 0.0));
	//backgrounds.push_back(Background(2, 1.0));

	//Spaceship ship0(0);
	Ship0 ship0(0);
	Ship1 ship1(1);
	//Spaceship ship2(4);

	camera = Camera(2); //2 for the number of spaceships

	AsteroidSpawner asteroidSpawner;

	sf::Clock clock;

	//---starting phase---
	int startingTime = 3; //seconds remaining in starting phase
	Text startingText("Starting in 3", sf::Vector2f(0.5, 1.f / 3.f));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return ScreenName::none;
				break;

			case sf::Event::Resized:
				WIN_X_LEN = event.size.width;
				WIN_Y_LEN = event.size.height;
				camera.resetSize();
				ship0.resetUIPositions();
				ship1.resetUIPositions();
				startingText.resetPosition();
				break;

			default:
				break;
			}
		}
		window.clear();

		if (clock.getElapsedTime().asSeconds() >= 1.0)
		{
			clock.restart();
			startingTime--;
			if (startingTime <= 0)
			{
				break;
			}
			startingText.setText("Starting in " + std::to_string(startingTime));
		}

		ship0.update();
		ship1.update();

		//---update the camera view---
		camera.coordinates[0] = ship0.position;
		camera.coordinates[1] = ship1.position;
		camera.updateView();

		//---draw everything---
		window.setView(camera.view);
		//draw backgrounds
		//window.draw(background);
		for (Background& b : backgrounds)
		{
			b.update(camera.view.getCenter(), camera.view.getSize());
			b.draw(window);
		}
		//draw ships
		ship0.draw(window);
		ship1.draw(window);


		//window.draw(camera.viewOutline);
		//draw ui
		window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
		//std::cout << window.getDefaultView().getSize().x << ", " << window.getDefaultView().getSize().y << std::endl;
		window.draw(startingText);
		ship0.drawUI(window);
		ship1.drawUI(window);
		window.display();
	}

	//---main phase---
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return ScreenName::none;
				break;

			case sf::Event::Resized:
				WIN_X_LEN = event.size.width;
				WIN_Y_LEN = event.size.height;
				camera.resetSize();
				ship0.resetUIPositions();
				ship1.resetUIPositions();
				break;

			default:
				break;
			}
		}
		window.clear();
		
		//---ship inputs---
		ship0.handleInputs();
		ship1.handleInputs();
		//ship2.handleInputs();

		//---update ship positions---
		ship0.handleCollision(ship1);
		ship0.update();
		ship1.update();
		//ship2.update();

		//---update bullets---
		for (int i = 0; i < bullets.size(); i++)
		{
			if (bullets[i].update())
			{
				bullets.erase(bullets.begin() + i);
				i--;
				continue;
			}
			//collisions with ships
			if (ship0.handleCollision(bullets[i]) || ship1.handleCollision(bullets[i])/* || ship2.handleCollision(bullets[i])*/)
			{
				bullets.erase(bullets.begin() + i);
				i--;
				continue;
			}
			//collisions with asteroids
			for (Asteroid& a : asteroids)
			{
				if (a.handleCollision(bullets[i]))
				{
					bullets.erase(bullets.begin() + i);
					i--;
					break;
				}
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
			//ship2.handleCollision(lasers[2]);
		}

		//---update the camera view---
		camera.coordinates[0] = ship0.position;
		camera.coordinates[1] = ship1.position;
		//camera.coordinates[2] = ship2.position;
		camera.updateView();

		//---update asteroid spawner and asteroids---
		asteroidSpawner.updateCamLocation(camera.view);
		asteroidSpawner.updateSpawn();
		for (int i = 0; i < asteroids.size(); i++)
		{
			if (asteroids[i].update(camera.view))
			{
				asteroids.erase(asteroids.begin() + i);
				i--;
				continue;
			}
			//handle collisions between ships and asteroids
			ship0.handleCollision(asteroids[i]);
			ship1.handleCollision(asteroids[i]);
			//ship2.handleCollision(asteroids[i]);

			//handle collisions between asteroids
			for (int j = i + 1; j < asteroids.size(); j++)
			{
				asteroids[i].handleCollision(asteroids[j]);
			}
		}

		//---exit if there is a winner (one of the ships overheated)---
		if (ship0.isOverheat())
		{
			winner = 1;
			break;
		}
		else if (ship1.isOverheat())
		{
			winner = 0;
			break;
		}
		/*else if (ship2.heat > SHIP_MAX_HEAT)
		{
			winner = 999;
			break;
		}*/

		//---update ship markers---
		ship0.setMarkerPosition(camera.getRelativePosition(0));
		ship1.setMarkerPosition(camera.getRelativePosition(1));
		//ship2.setMarkerPosition(camera.getRelativePosition(2));

		//---draw everything---
		window.setView(camera.view);
		//window.setView(sf::View(sf::FloatRect(0, 0, WIN_X_LEN * 2, WIN_Y_LEN * 2)));
		//draw backgrounds
		//window.draw(background);
		for (Background& b : backgrounds)
		{
			b.update(camera.view.getCenter(), camera.view.getSize());
			b.draw(window);
		}
		//draw ships
		ship0.draw(window);
		ship1.draw(window);
		//ship2.draw(window);
		//draw asteroids
		for (Asteroid& a : asteroids)
		{
			a.draw(window);
		}
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
		window.draw(camera.viewOutline);
		//draw ui
		window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
		//std::cout << window.getDefaultView().getSize().x << ", " << window.getDefaultView().getSize().y << std::endl;
		ship0.drawUI(window);
		ship1.drawUI(window);

		window.display();
	}

	//---ending phase---
	clock.restart();
	Text endingText("PLAYER " + std::to_string(winner + 1) + " WINS", sf::Vector2f(0.5, 1.f / 3.f));
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				return ScreenName::none;
				break;

			case sf::Event::Resized:
				WIN_X_LEN = event.size.width;
				WIN_Y_LEN = event.size.height;
				camera.resetSize();
				ship0.resetUIPositions();
				ship1.resetUIPositions();
				endingText.resetPosition();
				break;

			default:
				break;
			}
		}
		window.clear();

		if (clock.getElapsedTime().asSeconds() >= 3.0)
		{
			return ScreenName::end;
		}

		ship0.updateLocation();
		ship1.updateLocation();

		//---update the camera view---
		camera.coordinates[0] = ship0.position;
		camera.coordinates[1] = ship1.position;
		camera.updateView();

		//---draw everything---
		window.setView(camera.view);
		//draw backgrounds
		//window.draw(background);
		for (Background& b : backgrounds)
		{
			b.update(camera.view.getCenter(), camera.view.getSize());
			b.draw(window);
		}
		//draw ships
		ship0.draw(window);
		ship1.draw(window);


		//window.draw(camera.viewOutline);
		//draw ui
		window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
		window.draw(endingText);
		//std::cout << window.getDefaultView().getSize().x << ", " << window.getDefaultView().getSize().y << std::endl;
		ship0.drawUI(window);
		ship1.drawUI(window);
		window.display();
	}
	return ScreenName::none;
}