#include "Camera.h"
#include <iostream>

const float CAMERA_OBJECT_MAX_DISTANCE_X = 0.8; //multiplier for how far an object can be from the center of the camera
const float CAMERA_OBJECT_MAX_DISTANCE_Y = (1 - (1 - ((float)WIN_Y_LEN / WIN_X_LEN) * CAMERA_OBJECT_MAX_DISTANCE_X) / 2);
const float CAMERA_OBJECT_MIN_DISTANCE = 0.4;
const float CAMERA_ZOOM_INTENSITY = 0.1;

const sf::Vector2f CAMERA_MIN_SIZE(WIN_X_LEN / 2, WIN_Y_LEN / 2);

int temp = 10;

Camera::Camera(int initialCoordSize)
{
	view.reset(sf::FloatRect(0.f, 0.f, WIN_X_LEN, WIN_Y_LEN));

	coordinates.resize(initialCoordSize);
}

void Camera::updateView()
{
	//calculate desired center coordinate (average of all coordinates)
	sf::Vector2f center;
	for (sf::Vector2f p : coordinates)
	{
		center.x += p.x;
		center.y += p.y;
	}
	center.x /= coordinates.size();
	center.y /= coordinates.size();

	view.setCenter(center);

	for (sf::Vector2f p : coordinates)
	{
		temp++;

		//---zooming out---
		float xDist = abs(p.x - center.x); //distance from point to center
		float xDistMax = (view.getSize().x / 2) * CAMERA_OBJECT_MAX_DISTANCE_X;
		if (xDist > xDistMax)
		{
			float distToEdge = (view.getSize().x / 2) - xDist; //distance to edge of screen
			float distToEdgeMax = (view.getSize().x / 2) * (1 - CAMERA_OBJECT_MAX_DISTANCE_X); //maximum distance to edge of screen

			float speed = 1 - (distToEdge / distToEdgeMax);
			view.zoom(1 + (xDist / xDistMax - 1) * speed * CAMERA_ZOOM_INTENSITY);
		}
		
		float yDist = abs(p.y - center.y); //distance from point to center
		float yDistMax = (view.getSize().y / 2) * CAMERA_OBJECT_MAX_DISTANCE_Y;
		if (yDist > yDistMax)
		{
			float distToEdge = (view.getSize().y / 2) - yDist; //distance to edge of screen
			float distToEdgeMax = (view.getSize().y / 2) * (1 - CAMERA_OBJECT_MAX_DISTANCE_Y); //maximum distance to edge of screen

			float speed = 1 - (distToEdge / distToEdgeMax);
			view.zoom(1 + (yDist / yDistMax - 1) * speed * CAMERA_ZOOM_INTENSITY);
		}

		//---zooming in---

		float dist = sqrt(xDist * xDist + yDist * yDist);
		float distMin = sqrt(((view.getSize().x / 2) * CAMERA_OBJECT_MIN_DISTANCE) * ((view.getSize().x / 2) * CAMERA_OBJECT_MIN_DISTANCE) +
							 ((view.getSize().y / 2) * CAMERA_OBJECT_MIN_DISTANCE) * ((view.getSize().y / 2) * CAMERA_OBJECT_MIN_DISTANCE));
		if (dist < distMin)
		{
			float speed = 1 - (dist / distMin);
			view.zoom(1 + (dist / distMin - 1) * speed * CAMERA_ZOOM_INTENSITY);
		}

		/*float xDistMin = (view.getSize().x / 2) * CAMERA_OBJECT_MIN_DISTANCE;
		float yDistMin = (view.getSize().y / 2) * CAMERA_OBJECT_MIN_DISTANCE;
		//std::cout << xDist << ", " << xDistMin << ", " << (xDist < xDistMin) << std::endl;
		if (xDist < xDistMin && yDist < yDistMin)
		{
			float speed = 1 - (xDist / xDistMin);
			view.zoom(1 + (xDist / xDistMin - 1) * speed * CAMERA_ZOOM_INTENSITY);
			

			yDistMin = (view.getSize().y / 2) * CAMERA_OBJECT_MIN_DISTANCE;
			speed = 1 - (yDist / yDistMin);
			view.zoom(1 + (yDist / yDistMin - 1) * speed * CAMERA_ZOOM_INTENSITY);

			std::cout << "zoom in x" << std::endl;
		}

		std::cout << yDist << ", " << yDistMin << ", " << (yDist < yDistMin) << std::endl;*/
		/*if (yDist < yDistMin)
		{
			float speed = 1 - (yDist / yDistMin);
			view.zoom(1 + (yDist / yDistMin - 1) * speed * CAMERA_ZOOM_INTENSITY);

			std::cout << "zoom in y" << std::endl;
		}*/

		//---keeping the camera at a minimum size
		if (view.getSize().x < CAMERA_MIN_SIZE.x)
		{
			view.zoom(CAMERA_MIN_SIZE.x / view.getSize().x);
		}
	}

}

sf::Vector2f Camera::getRelativePosition(int index)
{
	//the coordinate shifted left by the camera's left side coordinate,
	// divided by the length of the camera,
	// multiplied by the size of the window
	return sf::Vector2f((coordinates[index].x - (view.getCenter().x - view.getSize().x / 2)) / view.getSize().x * WIN_X_LEN,
						(coordinates[index].y - (view.getCenter().y - view.getSize().y / 2)) / view.getSize().y * WIN_Y_LEN);
}