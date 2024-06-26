#include "home.h"
#include <iostream>
#include <vector>

ScreenName homeScreen(sf::RenderWindow& window)
{
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromFile("Assets/Images/Background 5.png");
	backgroundTexture.setRepeated(true);
	sf::Sprite background;
	background.setTexture(backgroundTexture);
	background.setTextureRect(sf::IntRect(0, 0, 100000, 100000));

	Text titleText("SPACE GAME", sf::Vector2f(0.5, 1.f / 3.f));

	std::vector<Button> buttons;
	int buttonIndex = 0; //index of the currently selected button
	buttons.push_back(Button("PLAY", sf::Vector2f(0.3, 2.f / 3.f)));
	buttons.push_back(Button("EXIT", sf::Vector2f(0.7, 2.f / 3.f)));
	buttons[0].setSelected(true);

	window.setView(sf::View(sf::FloatRect(0.0, 0.0, WIN_X_LEN, WIN_Y_LEN)));
	titleText.resetPosition();
	for (Button& b : buttons)
	{
		b.resetPosition();
	}

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
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, WIN_X_LEN, WIN_Y_LEN)));
				titleText.resetPosition();
				for (Button& b : buttons)
				{
					b.resetPosition();
				}
				break;

			case sf::Event::JoystickButtonPressed:
				//left and right change the selected button
				buttons[buttonIndex].setSelected(false);
				if (event.joystickButton.button == 13) //right
				{
					buttonIndex++;
				}
				else if (event.joystickButton.button == 15) //left
				{
					buttonIndex--;
				}
				buttonIndex = (buttonIndex + buttons.size()) % buttons.size();
				buttons[buttonIndex].setSelected(true);

				//START presses the selected button
				if (event.joystickButton.button == 9) //START
				{
					switch (buttonIndex)
					{
					case 0: //play button
						return ScreenName::play;
						break;
					case 1: //exit button
						window.close();
						return ScreenName::none;
						break;
					default:
						break;
					}
				}
				break;

			default:
				break;
			}
		}
		window.clear();

		//---draw everything---
		window.draw(background);
		for (Button& b : buttons)
		{
			b.draw(window);
		}

		window.draw(titleText);

		window.display();
	}


	return ScreenName::none;
}