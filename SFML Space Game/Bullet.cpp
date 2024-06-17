#include "Bullet.h"

const float BULLET_RADIUS = 4.0;
const sf::Vector2f BULLET_SIZE(BULLET_RADIUS, BULLET_RADIUS * 5);
//const float BULLET_SPEED = 15.0;
const float BULLET_SPEED = 11.0;
const float BULLET_DAMAGE = 50.0;

const int BULLET_LIFESPAN = 180; //number of frames until bullet despawns

Bullet::Bullet(sf::Vector2f pos, sf::Vector2f vel, float rot)
{
	hitbox.setSize(BULLET_SIZE);
	hitbox.setFillColor(sf::Color::Red);
	hitbox.setOrigin(BULLET_SIZE.x / 2, BULLET_SIZE.y / 2);
	hitbox.setRotation(rot);

	collisionBox.setRadius(BULLET_RADIUS);
	collisionBox.setFillColor(sf::Color::Blue);
	collisionBox.setOrigin(BULLET_RADIUS, BULLET_RADIUS);

	position = pos;
	velocity = vel;

	rotation = rot;
	lifespan = BULLET_LIFESPAN;
}
Bullet::Bullet()
{
	hitbox.setSize(BULLET_SIZE);
	hitbox.setFillColor(sf::Color::Red);

	collisionBox.setRadius(BULLET_RADIUS);
	collisionBox.setFillColor(sf::Color::Blue);

	rotation = 0.0;
	lifespan = BULLET_LIFESPAN;
}


bool Bullet::update()
{
	position += velocity;
	hitbox.setPosition(position);
	collisionBox.setPosition(position);

	lifespan--;
	return lifespan == 0;
}

void Bullet::draw(sf::RenderWindow& window)
{
	//window.draw(collisionBox);
	window.draw(hitbox);
}