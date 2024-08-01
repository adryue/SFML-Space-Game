#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "spaceship.h"

const float JOYSTICK_THRESHOLD = 0.15; //the minimum value required to move (after the joystick is normalized)
const float JOYSTICK_X_MAX_VALUE = 70; //the maximum value from joystick x input
const float JOYSTICK_Z_MAX_VALUE = 60; //the maximum value from joystick Z input
const float JOYSTICK_R_MAX_VALUE = 60; //the maximum value from joystick R input
const float JOYSTICK_U_MAX_VALUE = 75;
const float JOYSTICK_V_MAX_VALUE = 75;

//TODO: add spaceship classes

ShipConstants::ShipConstants()
{
	SHIP_SIZE = sf::Vector2f(40, 40);
	SHIP_RADIUS = (SHIP_SIZE.x + SHIP_SIZE.y) / 4; //used for calculating collisions
	SHIP_EXPANDED_MAX_SIZE = sf::Vector2f(160, 160);
	SHIP_EXPANDED_MAX_RADIUS = (SHIP_EXPANDED_MAX_SIZE.x + SHIP_EXPANDED_MAX_SIZE.y) / 4;

	SHIP_MASS = 10.0;

	SHIP_MARKER_SIZE = sf::Vector2f(10.0, 20.0);
	SHIP_MARKER_OFFSET = 40.0; //offset from the ship's position
	SHIP_MARKER_CIRCLE_RADIUS = 40.0;

	SHIP_ROTATION_SPEED = 3.5;
	SHIP_MOVEMENT_SPEED = 0.1;
	SHIP_MINI_MOVEMENT_SPEED = 0.01;
	SHIP_MEGA_MOVEMENT_SPEED = 0.3f;

	SHIP_MAX_HEAT = 300;
	SHIP_HEAT_DISSIPATION = 0.5;
	SHIP_HEAT_EXPANDED_MAX_DISSIPATION = 1.0;
	SHIP_MEGA_BOOST_HEAT = 1.1f;

	SHIP_COLLISION_HEAT_MULTIPLIER = 3.0;
	SHIP_COLLISION_SLOWDOWN_MULTIPLIER = 0.9; //collisions aren't perfectly elastic, as some energy becomes heat
	SHIP_COLLISION_CAMERA_SHAKE = 0.7f;

	SHIP_BULLET_FIRING_KNOCKBACK = 0.01; //multiplier value based on bullet's speed
	SHIP_BULLET_CAMERA_SHAKE = 2.5f;

	SHIP_LASER_FIRING_KNOCKBACK = 1.3; //multiplier value based on laser damage
	SHIP_LASER_MAX_BUILDUP = 100; //number of frames required to build up to full strength
	SHIP_LASER_BUILDUP_MAX_HEAT = 1.0; //when charging up the laser, the ship gains heat
	SHIP_LASER_CAMERA_SHAKE = 0.08f;
}

Spaceship::Spaceship(int ShipNumber) : Spaceship(ShipNumber, ShipConstants()) {}

Spaceship::Spaceship(int ShipNumber, ShipConstants constants) : CONSTANTS(constants),
															heatBar(HEATBAR_MAX_SIZE, constants.SHIP_MAX_HEAT, ShipNumber, 0, 0.f),
															laserBar(LASERBAR_MAX_SIZE, constants.SHIP_LASER_MAX_BUILDUP, ShipNumber, 1, HEATBAR_MAX_SIZE.y)
{
	heatBar.setMinMaxColors(sf::Color(255, 255, 0), sf::Color(255, 0, 0));
	//heatBar.setMinMaxColors(sf::Color(255, 255, 255), sf::Color(255, 0, 0));
	laserBar.setMinMaxColors(sf::Color::Cyan, sf::Color::Cyan);

	hitbox.setSize(CONSTANTS.SHIP_SIZE);
	hitbox.setOrigin(CONSTANTS.SHIP_SIZE.x / 2, CONSTANTS.SHIP_SIZE.y / 2);
	hitbox.setFillColor(sf::Color::White);

	collisionBox.setRadius(CONSTANTS.SHIP_RADIUS);
	collisionBox.setOrigin(CONSTANTS.SHIP_RADIUS, CONSTANTS.SHIP_RADIUS);
	//collisionBox.setFillColor(sf::Color::Green);
	collisionBox.setFillColor(sf::Color(100, 100, 100, 100));
	collisionBox.setOutlineThickness(3.0);
	collisionBox.setOutlineColor(sf::Color::White);

	//hitbox.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	position = sf::Vector2f(WIN_X_LEN / 2.0, WIN_Y_LEN / 2.0);

	texture.loadFromFile("Assets/Images/Blue Ship.png");
	sprite.setTexture(texture);
	sprite.setOrigin(CONSTANTS.SHIP_SIZE.x / 2, CONSTANTS.SHIP_SIZE.y / 2);

	thrusterFireTexture.loadFromFile("Assets/Images/Thruster Fire.png");
	thrusterFireSprite.setTexture(thrusterFireTexture);
	thrusterFireSprite.setOrigin(thrusterFireTexture.getSize().x / 2, 0);

	marker.setPointCount(3);
	marker.setPoint(0, sf::Vector2f(0.0, 0.0));
	marker.setPoint(1, sf::Vector2f(CONSTANTS.SHIP_MARKER_SIZE.x / 2, CONSTANTS.SHIP_MARKER_SIZE.y));
	marker.setPoint(2, sf::Vector2f(-CONSTANTS.SHIP_MARKER_SIZE.x / 2, CONSTANTS.SHIP_MARKER_SIZE.y));
	marker.setOrigin(0.0, CONSTANTS.SHIP_MARKER_SIZE.y + CONSTANTS.SHIP_MARKER_CIRCLE_RADIUS);
	marker.setFillColor(sf::Color::Magenta);

	markerCircle.setRadius(CONSTANTS.SHIP_MARKER_CIRCLE_RADIUS);
	markerCircle.setOrigin(CONSTANTS.SHIP_MARKER_CIRCLE_RADIUS, CONSTANTS.SHIP_MARKER_CIRCLE_RADIUS);
	markerCircle.setFillColor(sf::Color::Transparent);
	markerCircle.setOutlineThickness(3.0);
	markerCircle.setOutlineColor(sf::Color::Magenta);

	rotation = 0;

	heat = 0;

	laserBuildup = 0;
	isFiringLaser = false;

	isThrusting = false;
	drawMarker = false;

	heatDissipationMultiplier = 0;

	shipNumber = ShipNumber;
	switch (shipNumber)
	{
	case 0:
		position = sf::Vector2f(WIN_X_LEN / 3.0, WIN_Y_LEN / 2.0);
		break;
	case 1:
		position = sf::Vector2f(WIN_X_LEN / 3.0 * 2.0, WIN_Y_LEN / 2.0);
		break;
	default:
		position = sf::Vector2f((float)WIN_X_LEN / 2.f, (float)WIN_Y_LEN / 2.f);
		break;
	}

	state = State::normal;
}

void Spaceship::handleInputs()
{
	handleRotations();
	handleThrusting();
	handleHeatShield();
	handleFiring();

	//draw / don't draw marker
	drawMarker = sf::Joystick::isButtonPressed(shipNumber, 7);

	//reset button
	if (sf::Joystick::isButtonPressed(shipNumber, 9))
	{
		position = sf::Vector2f(WIN_X_LEN / 2.0, WIN_Y_LEN / 2.0);
		velocity.x = 0;
		velocity.y = 0;
	}
}

void Spaceship::handleRotations()
{
	float joyStickX = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::X);
	joyStickX = std::min(joyStickX, JOYSTICK_X_MAX_VALUE);
	joyStickX = std::max(joyStickX, -JOYSTICK_X_MAX_VALUE);
	joyStickX /= JOYSTICK_X_MAX_VALUE;
	
	if (abs(joyStickX) > JOYSTICK_THRESHOLD)
	{
		rotation += CONSTANTS.SHIP_ROTATION_SPEED * joyStickX;
	}
}

void Spaceship::handleThrusting()
{
	//---moving the ship forward---
	float joyStickU = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::U);
	joyStickU += JOYSTICK_U_MAX_VALUE;
	joyStickU = std::min(joyStickU, JOYSTICK_U_MAX_VALUE * 2);
	joyStickU = std::max(joyStickU, 0.0f);
	joyStickU /= JOYSTICK_U_MAX_VALUE * 2;
	if (joyStickU >= JOYSTICK_THRESHOLD)
	{
		isThrusting = true;

		//convert from degrees to radians
		float rot = rotation * M_PI / 180;

		if (!sf::Joystick::isButtonPressed(shipNumber, 4))
		{
			velocity.x += sin(rot) * CONSTANTS.SHIP_MOVEMENT_SPEED * joyStickU;
			velocity.y -= cos(rot) * CONSTANTS.SHIP_MOVEMENT_SPEED * joyStickU;
		}
		else //mega boost if the left trigger is held all the way down
		{
			velocity.x += sin(rot) * CONSTANTS.SHIP_MEGA_MOVEMENT_SPEED * joyStickU;
			velocity.y -= cos(rot) * CONSTANTS.SHIP_MEGA_MOVEMENT_SPEED * joyStickU;

			//mega boost builds up heat
			damage(CONSTANTS.SHIP_MEGA_BOOST_HEAT);
		}

		//show more of the thruster fire sprite if the trigger is pressed more
		thrusterFireSprite.setOrigin(thrusterFireTexture.getSize().x / 2,
									(-CONSTANTS.SHIP_RADIUS * 0.7 + thrusterFireTexture.getSize().y) - (thrusterFireTexture.getSize().y * joyStickU));
	}
	else
	{
		isThrusting = false;
	}

	//---mini thruster adjustments---
	float joyStickZ = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::Z);
	joyStickZ = std::min(joyStickZ, JOYSTICK_Z_MAX_VALUE);
	joyStickZ = std::max(joyStickZ, -JOYSTICK_Z_MAX_VALUE);
	joyStickZ /= JOYSTICK_Z_MAX_VALUE;
	float joyStickR = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::R);
	joyStickR = std::min(joyStickR, JOYSTICK_R_MAX_VALUE);
	joyStickR = std::max(joyStickR, -JOYSTICK_R_MAX_VALUE);
	joyStickR /= JOYSTICK_R_MAX_VALUE;

	float joyStickZR = magnitude(sf::Vector2f(joyStickZ, joyStickR));
	if (joyStickZR > JOYSTICK_THRESHOLD)
	{
		velocity.x -= CONSTANTS.SHIP_MINI_MOVEMENT_SPEED * joyStickR;
		velocity.y -= CONSTANTS.SHIP_MINI_MOVEMENT_SPEED * joyStickZ;

		//position.x -= SHIP_MOVEMENT_SPEED * joyStickR * 40;
		//position.y -= SHIP_MOVEMENT_SPEED * joyStickZ * 40;
		//velocity.x = 0;
		//velocity.y = 0;
	}
}
void Spaceship::handleHeatShield()
{
	//---changing the state to normal / expanded---
	float joyStickV = sf::Joystick::getAxisPosition(shipNumber, sf::Joystick::V);
	joyStickV += JOYSTICK_V_MAX_VALUE;
	joyStickV = std::min(joyStickV, JOYSTICK_V_MAX_VALUE * 2);
	joyStickV = std::max(joyStickV, 0.0f);
	joyStickV /= JOYSTICK_U_MAX_VALUE * 2;
	if (joyStickV >= JOYSTICK_THRESHOLD)
	{
		hitbox.setSize(sf::Vector2f(CONSTANTS.SHIP_SIZE.x + CONSTANTS.SHIP_EXPANDED_MAX_SIZE.x * joyStickV,
									CONSTANTS.SHIP_SIZE.y + CONSTANTS.SHIP_EXPANDED_MAX_SIZE.y * joyStickV));
		hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);

		collisionBox.setRadius(CONSTANTS.SHIP_RADIUS + CONSTANTS.SHIP_EXPANDED_MAX_RADIUS * joyStickV);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());
		
		heatDissipationMultiplier = joyStickV;
		state = State::expanded;
	}
	else
	{
		hitbox.setSize(CONSTANTS.SHIP_SIZE);
		hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);

		collisionBox.setRadius(CONSTANTS.SHIP_RADIUS);
		collisionBox.setOrigin(collisionBox.getRadius(), collisionBox.getRadius());

		state = State::normal;
	}
}

#if 0
void Spaceship::handleFiring()
{
	//the ship can only fire if it is not expanded
	if (state != State::normal)
	{
		laserBuildup = 0;
		laserBar.setValue(0);
		return;
	}

	//---firing a bullet---
	if (sf::Joystick::isButtonPressed(shipNumber, 1))
	{
		if (!isFiringBullet)
		{
			isFiringBullet = true;
			Bullet b(position, velocity, rotation); //bullet has the same starting position and velocity as the ship

			float rot = rotation * M_PI / 180;

			b.position.x += sin(rot) * collisionBox.getRadius();
			b.position.y -= cos(rot) * collisionBox.getRadius();

			b.velocity.x += sin(rot) * BULLET_SPEED;
			b.velocity.y -= cos(rot) * BULLET_SPEED;

			//ship gets knockback
			velocity.x -= sin(rot) * BULLET_SPEED * CONSTANTS.SHIP_BULLET_FIRING_KNOCKBACK;
			velocity.y += cos(rot) * BULLET_SPEED * CONSTANTS.SHIP_BULLET_FIRING_KNOCKBACK;

			addBullet(b);
		}
	}
	else
	{
		isFiringBullet = false;
	}

	//---firing a laser
	if (sf::Joystick::isButtonPressed(shipNumber, 2))
	{
		isFiringLaser = true;
		laserBuildup++;
		laserBuildup = std::min(CONSTANTS.SHIP_LASER_MAX_BUILDUP, laserBuildup);

		laserBar.setValue(laserBuildup);

		//charging up the laser also adds heat
		damage(CONSTANTS.SHIP_LASER_BUILDUP_MAX_HEAT * ((float)laserBuildup / CONSTANTS.SHIP_LASER_MAX_BUILDUP));
	}
	else
	{
		if (isFiringLaser)
		{
			isFiringLaser = false;
			float rot = rotation * M_PI / 180;
			float damage = (float)laserBuildup / CONSTANTS.SHIP_LASER_MAX_BUILDUP;

			Laser laser(sf::Vector2f(position.x + sin(rot) * collisionBox.getRadius(),
				position.y - cos(rot) * collisionBox.getRadius()),
				rotation,
				damage,
				sf::Vector2f(0.f, 0.f));

			addLaser(laser);

			laserBuildup = 0;
			laserBar.setValue(0);

			//ship gets knockback
			velocity.x -= sin(rot) * damage * CONSTANTS.SHIP_LASER_FIRING_KNOCKBACK;
			velocity.y += cos(rot) * damage * CONSTANTS.SHIP_LASER_FIRING_KNOCKBACK;
		}
	}
}
#endif

void Spaceship::update()
{
	updateLocation();
	updateHeat();
}

void Spaceship::updateLocation()
{
	hitbox.setRotation(rotation);
	sprite.setRotation(rotation);

	position += velocity;
	hitbox.setPosition(position);
	collisionBox.setPosition(position);
	sprite.setPosition(position);

	if (isThrusting)
	{
		thrusterFireSprite.setRotation(rotation);
		thrusterFireSprite.setPosition(position);
	}
}
void Spaceship::updateHeat()
{
	switch (state)
	{
	case State::normal:
		heat -= CONSTANTS.SHIP_HEAT_DISSIPATION;
		break;
	case State::expanded:
		heat -= CONSTANTS.SHIP_HEAT_DISSIPATION + CONSTANTS.SHIP_HEAT_EXPANDED_MAX_DISSIPATION * heatDissipationMultiplier;
		break;
	}
	heat = std::max(heat, 0.0f);
	heatBar.setValue(heat);
}

void Spaceship::draw(sf::RenderWindow& window)
{
	switch (state)
	{
	case State::normal:
		break;
	case State::expanded:
		window.draw(collisionBox);
		break;
	}
	if (isThrusting)
	{
		window.draw(thrusterFireSprite);
	}
	//window.draw(collisionBox);
	//window.draw(hitbox);
	window.draw(sprite);
}

void Spaceship::drawUI(sf::RenderWindow& window)
{
	if (drawMarker)
	{
		window.draw(marker);
		window.draw(markerCircle);
	}
	heatBar.draw(window);
	laserBar.draw(window);
}

bool Spaceship::handleCollision(Bullet b)
{
	float posX = b.position.x - position.x;
	float posY = b.position.y - position.y;
	float minDistance = b.collisionBox.getRadius() + collisionBox.getRadius();
	if (posX * posX + posY * posY < minDistance * minDistance)
	{
		damage(BULLET_DAMAGE);
		addCameraShake(CONSTANTS.SHIP_BULLET_CAMERA_SHAKE);
		return true;
	}
	return false;
}

bool Spaceship::handleCollision(Laser l)
{
	//if the laser is facing the opposite direction, it's guaranteed to not hit
	float aX = sin(l.hitbox.getRotation() * M_PI / 180);
	float aY = -cos(l.hitbox.getRotation() * M_PI / 180);

	float bX = l.position.x - position.x;
	float bY = l.position.y - position.y;

	//TODO: fix ship being behind laser but should still take damage

	if (aX * bX + aY * bY > 0) //if the dot product is negative (for some reason it's positive instead)
	{
		return false;
	}

	//calculate the intersection point between the equation representing the laser and the normal line going through the ship's circle
	float x = ((l.slope * l.position.x) + l.position.y + (position.x / l.slope) - position.y) / (l.slope + 1 / l.slope);
	float y = l.slope * (l.position.x - x) + l.position.y;

	//distance from the circle to the laser line
	float distX = x - position.x;
	float distY = y - position.y;
	float dist = magnitude(sf::Vector2f(distX, distY));

	if (dist < l.size.x / 2 + collisionBox.getRadius())
	{
		damage(l.damage);
		addCameraShake(CONSTANTS.SHIP_LASER_CAMERA_SHAKE * l.damage);
		return true;
	}

	return false;
}

void Spaceship::handleCollision(Asteroid &a)
{
	sf::Vector2f normal(position.x - a.position.x, position.y - a.position.y);
	float distance = magnitude(normal);
	if (distance < collisionBox.getRadius() + a.radius)
	{
		//std::cout << "normal: (" << normal.x << ", " << normal.y << ")" << std::endl;

		//translate the ship so that it stops overlapping with the asteroid
		position += (normal / distance) * (collisionBox.getRadius() + a.radius - distance);

		float velocityMag = magnitude(velocity); //magnitude of velocity
		float astVelocityMag = magnitude(a.velocity); //magnitude of asteroid velocity

		//get the tangent line
		sf::Vector2f tangent(normal.y, -normal.x);
		float tangentMag = magnitude(tangent); //magnitude of tangent line
		/*float thetaT = atan(tangent.x / -tangent.y);
		if (tangent.y > 0)
		{
			thetaT += M_PI;
		}*/
		float thetaT = getAngle(tangent);

		//get "horizontal" and "vertical" components projected along the tangent line
		/*float dotProduct = velocity.x * tangent.x + velocity.y * tangent.y;
		float theta = acos(dotProduct / (tangentMag * velocityMag));
		float verticalV = velocityMag * sin(theta);
		float horizontalV = dotProduct / tangentMag;*/
		float theta = thetaT - getAngle(velocity);
		float verticalV = velocityMag * sin(theta);
		float horizontalV = velocityMag * cos(theta);
		if (velocity.x == 0 && velocity.y == 0)
		{
			verticalV = 0;
			horizontalV = 0;
		}

		//do the same thing with the asteroid
		/*float astDotProduct = a.velocity.x * tangent.x + a.velocity.y * tangent.y;
		float astTheta = acos(astDotProduct / (tangentMag * astVelocityMag));
		float astVerticalV = -(astVelocityMag * sin(astTheta));
		float astHorizontalV = astDotProduct / tangentMag;*/
		float astTheta = thetaT - getAngle(a.velocity);
		float astVerticalV = astVelocityMag * sin(astTheta);
		float astHorizontalV = astVelocityMag * cos(astTheta);
		if (a.velocity.x == 0 && a.velocity.y == 0)
		{
			astVerticalV = 0;
			astHorizontalV = 0;
		}

		//calculate the new "vertical" velocities using conservation of momentum and kinetic energy
		float newVerticalV = (verticalV * (CONSTANTS.SHIP_MASS - a.mass) + 2 * a.mass * astVerticalV) / (CONSTANTS.SHIP_MASS + a.mass);
		float newAstVerticalV = verticalV - astVerticalV + newVerticalV;

		//collisions aren't perfectly elastic, some energy becomes heat
		damage(abs(verticalV - newVerticalV) * CONSTANTS.SHIP_COLLISION_HEAT_MULTIPLIER);
		addCameraShake(abs(verticalV - newVerticalV) * CONSTANTS.SHIP_COLLISION_CAMERA_SHAKE);
		newVerticalV *= CONSTANTS.SHIP_COLLISION_SLOWDOWN_MULTIPLIER;
		newAstVerticalV *= CONSTANTS.SHIP_COLLISION_SLOWDOWN_MULTIPLIER;

		//convert "horizontal" and "vertical" velocities back to x and y coordinates
		velocity.x = horizontalV * sin(thetaT) - newVerticalV * cos(thetaT);
		velocity.y = -horizontalV * cos(thetaT) - newVerticalV * sin(thetaT);

		if (isnan(velocity.x) || isnan(velocity.y))
		{
			std::cout << "Something's wrong" << std::endl;
		}

		//same with asteroids
		a.velocity.x = astHorizontalV * sin(thetaT) - newAstVerticalV * cos(thetaT);
		a.velocity.y = -astHorizontalV * cos(thetaT) - newAstVerticalV * sin(thetaT);
	}
}

void Spaceship::handleCollision(Spaceship& s) //this is almost exactly the same code as Spaceship::handleCollision(Asteroid &a)
{
	sf::Vector2f normal(position.x - s.position.x, position.y - s.position.y);
	float distance = magnitude(normal);
	if (distance < collisionBox.getRadius() + s.collisionBox.getRadius())
	{
		//std::cout << "normal: (" << normal.x << ", " << normal.y << ")" << std::endl;

		//translate the ship so that it stops overlapping with the other ship
		position += (normal / distance) * (collisionBox.getRadius() + s.collisionBox.getRadius() - distance);

		float velocityMag = magnitude(velocity); //magnitude of velocity
		float sVelocityMag = magnitude(s.velocity); //magnitude of other ship velocity

		//get the tangent line
		sf::Vector2f tangent(normal.y, -normal.x);
		float tangentMag = magnitude(tangent); //magnitude of tangent line
		/*float thetaT = atan(tangent.x / -tangent.y);
		if (tangent.y > 0)
		{
			thetaT += M_PI;
		}*/
		float thetaT = getAngle(tangent);

		//get "horizontal" and "vertical" components projected along the tangent line
		/*float dotProduct = velocity.x * tangent.x + velocity.y * tangent.y;
		float theta = acos(dotProduct / (tangentMag * velocityMag));
		float verticalV = velocityMag * sin(theta);
		float horizontalV = dotProduct / tangentMag;*/
		float theta = thetaT - getAngle(velocity);
		float verticalV = velocityMag * sin(theta);
		float horizontalV = velocityMag * cos(theta);
		if (velocity.x == 0 && velocity.y == 0)
		{
			verticalV = 0;
			horizontalV = 0;
		}

		//do the same thing with the other ship
		/*float sDotProduct = s.velocity.x * tangent.x + s.velocity.y * tangent.y;
		float sTheta = acos(sDotProduct / (tangentMag * sVelocityMag));
		float sVerticalV = -(sVelocityMag * sin(sTheta));
		float sHorizontalV = sDotProduct / tangentMag;*/
		float sTheta = thetaT - getAngle(s.velocity);
		float sVerticalV = sVelocityMag * sin(sTheta);
		float sHorizontalV = sVelocityMag * cos(sTheta);
		if (s.velocity.x == 0 && s.velocity.y == 0)
		{
			sVerticalV = 0;
			sHorizontalV = 0;
		}

		//calculate the new "vertical" velocities using conservation of momentum and kinetic energy
		float newVerticalV = sVerticalV;
		float newAstVerticalV = verticalV;

		//collisions aren't perfectly elastic, some energy becomes heat
		damage(abs(verticalV - newVerticalV) * CONSTANTS.SHIP_COLLISION_HEAT_MULTIPLIER);
		s.damage(abs(verticalV - newVerticalV) * CONSTANTS.SHIP_COLLISION_HEAT_MULTIPLIER);
		addCameraShake(abs(verticalV - newVerticalV) * CONSTANTS.SHIP_COLLISION_CAMERA_SHAKE);
		newVerticalV *= CONSTANTS.SHIP_COLLISION_SLOWDOWN_MULTIPLIER;
		newAstVerticalV *= CONSTANTS.SHIP_COLLISION_SLOWDOWN_MULTIPLIER;

		//convert "horizontal" and "vertical" velocities back to x and y coordinates
		velocity.x = horizontalV * sin(thetaT) - newVerticalV * cos(thetaT);
		velocity.y = -horizontalV * cos(thetaT) - newVerticalV * sin(thetaT);

		if (isnan(velocity.x) || isnan(velocity.y))
		{
			std::cout << "Something's wrong" << std::endl;
		}

		//same with asteroids
		s.velocity.x = sHorizontalV * sin(thetaT) - newAstVerticalV * cos(thetaT);
		s.velocity.y = -sHorizontalV * cos(thetaT) - newAstVerticalV * sin(thetaT);
	}
}

void Spaceship::damage(float amount)
{
	heat += amount;
	heatBar.setValue(heat);
}

bool Spaceship::isOverheat()
{
	return heat > CONSTANTS.SHIP_MAX_HEAT;
}

void Spaceship::setMarkerPosition(sf::Vector2f pos)
{
	//marker.setPosition(pos.x, pos.y + SHIP_MARKER_OFFSET);
	marker.setPosition(pos);
	marker.setRotation(rotation);

	markerCircle.setPosition(pos);
}

void Spaceship::resetUIPositions()
{
	heatBar.resetPosition();
	laserBar.resetPosition();
}

/*
	For gamecube controllers
	Left Joystick:
		X Axis (left is -74, right is 78) -80 88
		Y Axis (up is -84, down is 69) -85 76
	Right Joystick:
		Z Axis (down is -74, up is 69) -64 80
		R Axis (left is 61, right is -75) 69 -78
	Left Trigger:
		U Axis (unpressed is -76, fully pressed is 80)
	Right Trigger:
		V Axis (unpressed is -75, fully pressed is 80)
	Buttons:
		0: X
		1: A
		2: B
		3: Y
		4: Left Trigger
		5: Right Trigger
		7: Z Trigger
		9: Start
		12: Up
		13: Right
		14: Down
		15: Left
*/