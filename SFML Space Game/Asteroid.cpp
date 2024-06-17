#include "Asteroid.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

const int ASTEROID_DESPAWN_TIME = 600; //number of frames asteroid is out of view before despawning

const float ASTEROID_MASS_MULTIPLIER = 0.00005;

const float ASTEROID_COLLISION_SLOWDOWN_MULTIPLIER = 0.9;

Asteroid::Asteroid(float radi, sf::Vector2f pos, sf::Vector2f vel)
{
	collisionBox.setFillColor(sf::Color(100, 100, 100));
	collisionBox.setRadius(radi);
	collisionBox.setOrigin(radi, radi);
	collisionBox.setPosition(pos);

	mass = (4.f / 3.f) * M_PI * radi * radi * radi * ASTEROID_MASS_MULTIPLIER;
	//mass = 10;
	radius = radi;
	position = pos;
	velocity = vel;

	outOfViewCount = 0;
}

bool Asteroid::update(sf::View camView)
{
	position += velocity;
	collisionBox.setPosition(position);

	if (position.x + radius < camView.getCenter().x - camView.getSize().x / 2 ||
		position.x - radius > camView.getCenter().x + camView.getSize().x / 2 ||
		position.y + radius < camView.getCenter().y - camView.getSize().y / 2 ||
		position.y - radius > camView.getCenter().y + camView.getSize().y / 2)
	{
		outOfViewCount++;
		return outOfViewCount >= ASTEROID_DESPAWN_TIME;
	}
	else
	{
		outOfViewCount = 0;
	}
	return false;
}

bool Asteroid::handleCollision(Bullet b)
{
	return magnitude(position - b.position) < radius + b.collisionBox.getRadius();
}

void Asteroid::handleCollision(Asteroid& a) //this code is nearly the same as Spaceship::handleCollision(Asteroid& a)
{
	sf::Vector2f normal(position.x - a.position.x, position.y - a.position.y);
	float distance = magnitude(normal);
	if (distance < radius + a.radius)
	{
		//std::cout << "normal: (" << normal.x << ", " << normal.y << ")" << std::endl;

		//translate the asteroid so that it stops overlapping with the other asteroid
		position += (normal / distance) * (radius + a.radius - distance);

		float velocityMag = magnitude(velocity); //magnitude of velocity
		float astVelocityMag = magnitude(a.velocity); //magnitude of asteroid velocity

		//get the tangent line
		sf::Vector2f tangent(normal.y, -normal.x);
		float tangentMag = magnitude(tangent); //magnitude of tangent line
		float thetaT = atan(tangent.x / -tangent.y);
		if (tangent.y > 0)
		{
			thetaT += M_PI;
		}

		//get "horizontal" and "vertical" components projected along the tangent line
		float dotProduct = velocity.x * tangent.x + velocity.y * tangent.y;
		float theta = acos(dotProduct / (tangentMag * velocityMag));
		float verticalV = velocityMag * sin(theta);
		float horizontalV = dotProduct / tangentMag;
		if (velocity.x == 0 && velocity.y == 0)
		{
			verticalV = 0;
			horizontalV = 0;
		}

		//do the same thing with the other asteroid
		float astDotProduct = a.velocity.x * tangent.x + a.velocity.y * tangent.y;
		float astTheta = acos(astDotProduct / (tangentMag * astVelocityMag));
		float astVerticalV = -(astVelocityMag * sin(astTheta));
		float astHorizontalV = astDotProduct / tangentMag;
		if (a.velocity.x == 0 && a.velocity.y == 0)
		{
			astVerticalV = 0;
			astHorizontalV = 0;
		}

		//calculate the new "vertical" velocities using conservation of momentum and kinetic energy
		float newVerticalV = (verticalV * (mass - a.mass) + 2 * a.mass * astVerticalV) / (mass + a.mass);
		float newAstVerticalV = verticalV - astVerticalV + newVerticalV;

		//collisions aren't perfectly elastic, some energy becomes heat
		newVerticalV *= ASTEROID_COLLISION_SLOWDOWN_MULTIPLIER;
		newAstVerticalV *= ASTEROID_COLLISION_SLOWDOWN_MULTIPLIER;

		//convert "horizontal" and "vertical" velocities back to x and y coordinates
		velocity.x = horizontalV * sin(thetaT) - newVerticalV * cos(thetaT);
		velocity.y = -horizontalV * cos(thetaT) - newVerticalV * sin(thetaT);

		if (isnan(velocity.x) || isnan(velocity.y))
		{
			std::cout << "Something's wrong" << std::endl;
		}

		//same with other asteroid
		a.velocity.x = astHorizontalV * sin(thetaT) - newAstVerticalV * cos(thetaT);
		a.velocity.y = -astHorizontalV * cos(thetaT) - newAstVerticalV * sin(thetaT);
	}
}


void Asteroid::draw(sf::RenderWindow& window)
{
	window.draw(collisionBox);
}