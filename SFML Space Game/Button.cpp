#include "Button.h"
#include <iostream>

const float BUTTON_PADDING = 30.f;

const sf::Color BUTTON_DEFAULT_COLOR(30, 30, 30, 128);
const sf::Color BUTTON_SELECTED_COLOR(120, 120, 120, 128);

Button::Button(std::string txt, sf::Vector2f orient) : text(txt, orient)
{
	orientation = orient;

	hitbox.setFillColor(BUTTON_DEFAULT_COLOR);
	hitbox.setOutlineColor(sf::Color::White);
	hitbox.setSize(sf::Vector2f(text.getLocalBounds().width + text.getLocalBounds().left + BUTTON_PADDING,
								text.getLocalBounds().height + text.getLocalBounds().top + BUTTON_PADDING));
	hitbox.setOrigin(hitbox.getSize().x / 2, hitbox.getSize().y / 2);
	hitbox.setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}

void Button::resetPosition()
{
	text.resetPosition();
	hitbox.setPosition(WIN_X_LEN * orientation.x, WIN_Y_LEN * orientation.y);
}

void Button::setSelected(bool selected)
{
	if (selected)
	{
		//text.setStyle(sf::Text::Underlined);
		hitbox.setOutlineThickness(2.0);
		hitbox.setFillColor(BUTTON_SELECTED_COLOR);
	}
	else
	{
		//text.setStyle(sf::Text::Regular);
		hitbox.setOutlineThickness(0);
		hitbox.setFillColor(BUTTON_DEFAULT_COLOR);
	}
}

void Button::draw(sf::RenderWindow& window)
{
	window.draw(hitbox);
	window.draw(text);

	/*sf::RectangleShape temp(sf::Vector2f(4, 4));
	temp.setFillColor(sf::Color::Red);
	temp.setOrigin(2, 2);
	temp.setPosition(WIN_X_LEN / 2, WIN_Y_LEN / 2);
	window.draw(temp);*/
}