#include "FlashCard.hpp"
#include "Platform/Platform.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
	util::Platform platform;
	srand((unsigned long)time(NULL));
#if defined(_DEBUG)
	std::cout << "Hello World and Teamates!" << std::endl;
#endif

	sf::RenderWindow window;

	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(600.0f * screenScalingFactor, 600.0f * screenScalingFactor), "MENU!", sf::Style::Close | sf::Style::Titlebar);
	// sf::RectangleShape shape(sf::Vector2f(100.0f, 50.0f));
	sf::Font font;
	font.loadFromFile("src/Font/JetBrainsMono-VariableFont_wght.ttf");
	FlashCard test("What is 2+2?", "4");

	// test.getCardA().setSize(sf::Vector2f(100.0f, 50.0f));
	// test.getCardQ().setSize(sf::Vector2f(100.0f, 50.0f));
	// test.getCardA().setOrigin(50.0f, 25.0f);
	// test.getCardQ().setOrigin(50.0f, 25.0f);

	// test.getCardA().setFillColor(sf::Color::White);
	// test.getCardQ().setFillColor(sf::Color::White);
	// shape.setOrigin(50.0f, 25.0f);
	sf::Text text;

	text.setFont(font);
	text.setOrigin(sf::Vector2f(text.getCharacterSize(), text.getCharacterSize() / 2));
	text.setString("Testly");
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		// sf::Event::textEntered can be used to track user input
		// sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) can be used to track user movement rather than input (WASD)
		// shape.move(-.01f, 0.0f) moves left for example (second value needs to be negative to move up)
		// we have to clear the screen regularly

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			test.setCardPos('Q', pos);

			std::cout << "x: " << mousePos.x << " y: " << mousePos.y << std::endl;
		}
		if (test.checkCollision())
		{
			std::cout << "Correct!\n";
		}

		window.clear();
		// window.draw(test.getCardQ());
		// window.draw(test.getCardA());
		test.draw(window, font);
		window.draw(text);
		window.display();
	}

	return 0;
}