#include "FlashCard.hpp"
#include "Platform/Platform.hpp"
#include "Textbox.hpp"
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
	//window.setKeyRepeatEnabled(true);

	TextBox tbox1(16, sf::Color::Black, false);
	tbox1.setFont(font);
	tbox1.setpos({100,100});

	TextBox tbox2(16, sf::Color::Black, false);
	tbox2.setFont(font);
	tbox2.setpos({100,200});
	//FlashCard test("What is 2+2?", "4");

	// test.getCardA().setSize(sf::Vector2f(100.0f, 50.0f));
	// test.getCardQ().setSize(sf::Vector2f(100.0f, 50.0f));
	// test.getCardA().setOrigin(50.0f, 25.0f);
	// test.getCardQ().setOrigin(50.0f, 25.0f);

	// test.getCardA().setFillColor(sf::Color::White);
	// test.getCardQ().setFillColor(sf::Color::White);
	// shape.setOrigin(50.0f, 25.0f);
	sf::Text text;

	/* text.setFont(font);
	text.setOrigin(sf::Vector2f(text.getCharacterSize(), text.getCharacterSize() / 2));
	text.setString("Testly");
	text.setCharacterSize(16);
	text.setFillColor(sf::Color::White);
 */
	sf::Text t1, t2;
	t1.setFont(font);
	t1.setCharacterSize(20);
	t1.setString("Term");
	sf::RectangleShape rect1(sf::Vector2f(400,40));

	t2.setFont(font);
	t2.setCharacterSize(20);
	t2.setString("Definition");
	sf::RectangleShape rect2(sf::Vector2f(400,200));

	t1.setPosition({100,75});
	t2.setPosition({100,175});

	rect1.setPosition({100,100});
	rect2.setPosition({100,200});

	while (window.isOpen())
	{
		sf::Event event;

		sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if(rect1.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				tbox2.setSelected(false);
				tbox1.setSelected(true);
			}

			else if(rect2.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				tbox1.setSelected(false);
				tbox2.setSelected(true);
			}

			else
			{
				tbox1.setSelected(false);
				tbox2.setSelected(false);
			}


		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			tbox1.setSelected(false);
			tbox2.setSelected(false);
		}


		while (window.pollEvent(event))
		{
				switch (event.type)
				{
				default:
					break;
				case sf::Event::Closed:
					window.close();
					break;
				case sf::Event::TextEntered:
					tbox1.typedOn(event);
					tbox2.typedOn(event);
				}

		}
		// sf::Event::textEntered can be used to track user input
		// sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) can be used to track user movement rather than input (WASD)
		// shape.move(-.01f, 0.0f) moves left for example (second value needs to be negative to move up)
		// we have to clear the screen regularly

		/* if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			test.setCardPos('Q', pos);

			std::cout << "x: " << mousePos.x << " y: " << mousePos.y << std::endl;
		}
		if (test.checkCollision())
		{
			std::cout << "Correct!\n";
		} */


		window.clear();

		window.draw(rect1);
		window.draw(rect2);
		window.draw(t1);
		window.draw(t2);

		tbox1.drawTo(window);
		tbox2.drawTo(window);

		// window.draw(test.getCardQ());
		// window.draw(test.getCardA());
		//test.draw(window, font);
		//window.draw(text);
		window.display();
	}

	return 0;
}