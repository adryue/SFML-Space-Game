#include "Utilities.h"
#define _USE_MATH_DEFINES
#include <math.h>

float magnitude(sf::Vector2f vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

float getAngle(sf::Vector2f vec)
{
	float angle = atan(vec.x / -vec.y);
	if (vec.y > 0)
	{
		angle += M_PI;
	}
	else if (vec.y == 0)
	{
		angle = vec.x > 0 ? M_PI / 2 : -M_PI / 2;
	}
	return angle;
}