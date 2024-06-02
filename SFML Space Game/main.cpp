#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Space Game");
	sf::RectangleShape rectangle(sf::Vector2f(100, 100));
	rectangle.setFillColor(sf::Color::White);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear();
		window.draw(rectangle);
		window.display();
	}
}