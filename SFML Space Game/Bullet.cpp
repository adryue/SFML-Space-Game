#include "Bullet.h"

const float BULLET_RADIUS = 3.0;
const float BULLET_SPEED = 10.0;
const float BULLET_DAMAGE = 2.0;

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f vel)
{
	hitbox.setRadius(BULLET_RADIUS);
	hitbox.setFillColor(sf::Color::Red);

	position = pos;
	velocity = vel;
}
Bullet::Bullet()
{
	hitbox.setRadius(BULLET_RADIUS);
	hitbox.setFillColor(sf::Color::Red);
}


void Bullet::update()
{
	position += velocity;
	hitbox.setPosition(position);
}

void Bullet::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
}