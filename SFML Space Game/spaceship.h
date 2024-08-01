#pragma once
#include <SFML/Graphics.hpp>
#include "CommonDef.h"
#include "play.h"
#include "Bullet.h"
#include "Laser.h"
#include "Healthbar.h"
#include "Asteroid.h"

extern const float JOYSTICK_THRESHOLD; //the minimum value required to move (after the joystick is normalized)
extern const float JOYSTICK_X_MAX_VALUE; //the maximum value from joystick x input
extern const float JOYSTICK_Z_MAX_VALUE; //the maximum value from joystick Z input
extern const float JOYSTICK_R_MAX_VALUE; //the maximum value from joystick R input
extern const float JOYSTICK_U_MAX_VALUE;
extern const float JOYSTICK_V_MAX_VALUE;

class ShipConstants
{
public:
	sf::Vector2f SHIP_SIZE;
	float SHIP_RADIUS; //used for calculating collisions
	sf::Vector2f SHIP_EXPANDED_MAX_SIZE;
	float SHIP_EXPANDED_MAX_RADIUS;

	float SHIP_MASS;

	sf::Vector2f SHIP_MARKER_SIZE;
	float SHIP_MARKER_OFFSET; //offset from the ship's position
	float SHIP_MARKER_CIRCLE_RADIUS;

	float SHIP_ROTATION_SPEED;
	float SHIP_MOVEMENT_SPEED;
	float SHIP_MINI_MOVEMENT_SPEED;
	float SHIP_MEGA_MOVEMENT_SPEED;

	float SHIP_MAX_HEAT;
	float SHIP_HEAT_DISSIPATION;
	float SHIP_HEAT_EXPANDED_MAX_DISSIPATION;
	float SHIP_MEGA_BOOST_HEAT;

	float SHIP_COLLISION_HEAT_MULTIPLIER = 3.0;
	float SHIP_COLLISION_SLOWDOWN_MULTIPLIER = 0.9; //collisions aren't perfectly elastic, as some energy becomes heat
	float SHIP_COLLISION_CAMERA_SHAKE = 0.7f;

	float SHIP_BULLET_FIRING_KNOCKBACK; //multiplier value based on bullet's speed
	float SHIP_BULLET_CAMERA_SHAKE;

	float SHIP_LASER_FIRING_KNOCKBACK; //multiplier value based on laser damage
	int SHIP_LASER_MAX_BUILDUP; //number of frames required to build up to full strength
	float SHIP_LASER_BUILDUP_MAX_HEAT; //when charging up the laser, the ship gains heat
	float SHIP_LASER_CAMERA_SHAKE;

	ShipConstants();
};

class Spaceship
{
public:

	ShipConstants CONSTANTS;

	sf::Texture texture;
	sf::Sprite sprite;
	sf::RectangleShape hitbox;
	sf::CircleShape collisionBox;

	sf::Texture thrusterFireTexture;
	sf::Sprite thrusterFireSprite;

	sf::ConvexShape marker; //marks where the ship is on the screen
	sf::CircleShape markerCircle;

	sf::Vector2f position;
	sf::Vector2f velocity;
	float rotation; //in degrees

	float heat;
	Healthbar heatBar;

	int laserBuildup;
	bool isFiringLaser;
	Healthbar laserBar;

	//these variables are only for checking to see if you should draw certain objects
	bool isThrusting;
	bool drawMarker;

	float heatDissipationMultiplier;

	int shipNumber; //also used for finding which controller to take input from

	enum State
	{
		normal,
		expanded
	};

	State state;

	Spaceship(int ShipNumber);
	Spaceship(int ShipNumber, ShipConstants constants);
	Spaceship(int ShipNumber, float maxHeat, float maxLaser);

	virtual void handleInputs();
	virtual void handleRotations();
	virtual void handleThrusting();
	virtual void handleHeatShield();
	virtual void handleFiring() = 0;

	void update();
	void updateLocation();
	virtual void updateHeat();

	void draw(sf::RenderWindow& window);
	void drawUI(sf::RenderWindow& window);

	bool handleCollision(Bullet b);
	bool handleCollision(Laser l);
	void handleCollision(Asteroid &a);
	void handleCollision(Spaceship& s);
	void damage(float amount);
	bool isOverheat();

	void setMarkerPosition(sf::Vector2f pos);
	void resetUIPositions();
};