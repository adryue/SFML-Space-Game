#include "Bullet.h"

Bullet::Bullet()
{

}

void Bullet::update()
{

}

void Bullet::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}