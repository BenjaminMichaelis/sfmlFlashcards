#include "FlashCard.hpp"
#include "Hash.hpp"
#include "Platform/Platform.hpp"
#include "RedBlackTree.hpp"
#include "Textbox.hpp"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <unistd.h>
#endif

// local setDefault function to iterate through hash calling FlashCard's setDefault()
void setDefault(Hash<int, FlashCard>& hash, sf::Font& font);
void setDefault(Hash<int, FlashCard>& hash, sf::Font& font)
{
	std::unordered_map<int, FlashCard>::iterator it = hash.sut.begin();
	for (; it != hash.sut.end(); it++)
		it->second.setDefault(font);
}

// local checkOpacity funcion to check if the flashcards used in match are all transparent, if not this returns false
bool checkOpacity(FlashCard& f1, FlashCard& f2, FlashCard& f3);
bool checkOpacity(FlashCard& f1, FlashCard& f2, FlashCard& f3)
{
	if ((!f1.checkOpacity() && !f2.checkOpacity()) && !f3.checkOpacity()) // return true if all three flashcards are transparent
		return true;

	return false;
}

// checks for general collisions between flashcards, none specific to match but required before starting it
void checkCollision(FlashCard& f1, FlashCard& f2, sf::Font& font);
void checkCollision(FlashCard& f1, FlashCard& f2, sf::Font& font)
{
	bool check = true;
	while (check)
	{
		sf::Vector2f halfsize = f1.getCardQ().getSize() / 2.0f;
		sf::Vector2f delta(std::abs(f1.getCardQ().getPosition().x - f2.getCardQ().getPosition().x), std::abs(f1.getCardQ().getPosition().y - f2.getCardQ().getPosition().y));
		sf::Vector2f intersect(delta.x - (halfsize.x + halfsize.x), delta.y - (halfsize.y + halfsize.y));
		if (intersect.x <= 0.0f && intersect.y <= 0.0f)
			f2.setDefault(font);
		else
			check = false;
	}
	check = true;
	while (check)
	{
		sf::Vector2f halfsize = f1.getCardA().getSize() / 2.0f;
		sf::Vector2f delta(std::abs(f1.getCardA().getPosition().x - f2.getCardA().getPosition().x), std::abs(f1.getCardA().getPosition().y - f2.getCardA().getPosition().y));
		sf::Vector2f intersect(delta.x - (halfsize.x + halfsize.x), delta.y - (halfsize.y + halfsize.y));
		if (intersect.x <= 0.0f && intersect.y <= 0.0f)
			f2.setDefault(font);
		else
			check = false;
	}
}

// local function that holds the logic for the match game: click event, correct answer collisions, incorrect answer collisions, etc.
void matchGameLogistics(FlashCard& flashCard, sf::RenderWindow& window, sf::Text& note, sf::Text& matchTitle, sf::Font& font, sf::Event& event, Hash<int, FlashCard>& cardDeck, int index1, int index2);
void matchGameLogistics(FlashCard& flashCard, sf::RenderWindow& window, sf::Text& note, sf::Text& matchTitle, sf::Font& font, sf::Event& event, Hash<int, FlashCard>& cardDeck, int index1, int index2)
{
	if (flashCard.getCardQ().getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
	{
		while (sf::Mouse::isButtonPressed(sf::Mouse::Left) && event.type != sf::Event::MouseButtonReleased)
		{
			sf::Vector2i mousePos = sf::Mouse::getPosition(window);
			sf::Vector2f pos(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
			flashCard.setCardPos('Q', pos);
			window.clear();
			window.draw(note);
			window.draw(matchTitle);
			flashCard.draw(window, font);
			cardDeck.sut[index1].draw(window, font);
			cardDeck.sut[index2].draw(window, font);
			window.display();
			if (flashCard.checkCollision()) // check for correct answer
			{
				flashCard.setCardColor(sf::Color::Green);
				window.clear();
				window.draw(note);
				window.draw(matchTitle);
				flashCard.draw(window, font);
				cardDeck.sut[index1].draw(window, font);
				cardDeck.sut[index2].draw(window, font);
				window.display();
				sleep(1);
				event.type = sf::Event::MouseButtonReleased;
				flashCard.setOpacity(false);
			}
			if (flashCard.checkCollision(cardDeck.sut[index1])) // check for incorrect answer
			{
				flashCard.setQColor(sf::Color::Red);
				cardDeck.sut[index1].setAColor(sf::Color::Red);
				for (int i = 0; i < 2; i++)
				{
					window.clear();
					window.draw(note);
					window.draw(matchTitle);
					flashCard.draw(window, font);
					cardDeck.sut[index1].draw(window, font);
					cardDeck.sut[index2].draw(window, font);
					window.display();
					sleep(1);
					if (i == 0)
					{
						flashCard.setQColor(sf::Color::White);
						cardDeck.sut[index1].setAColor(sf::Color::White);
						flashCard.moveCard('Q', { -10, 0 });
						cardDeck.sut[index2].moveCard('A', { 10, 0 });
					}
				}
			}
			if (flashCard.checkCollision(cardDeck.sut[index2])) // check for incorrect answer
			{
				flashCard.setQColor(sf::Color::Red);
				cardDeck.sut[index2].setAColor(sf::Color::Red);
				for (int i = 0; i < 2; i++)
				{
					window.clear();
					window.draw(note);
					window.draw(matchTitle);
					flashCard.draw(window, font);
					cardDeck.sut[index1].draw(window, font);
					cardDeck.sut[index2].draw(window, font);
					window.display();
					sleep(1);
					if (i == 0)
					{
						flashCard.setQColor(sf::Color::White);
						cardDeck.sut[index2].setAColor(sf::Color::White);
						flashCard.moveCard('Q', { -10, 0 });
						cardDeck.sut[index2].moveCard('A', { 10, 0 });
					}
				}
			}
		}
	}
}

int main()
{
	util::Platform platform;
	srand((unsigned long)time(NULL));
#if defined(_DEBUG)
	std::cout << "Hello World and Teamates!" << std::endl;
#endif
	//redblacktree
	// RedBlackTree<std::string, FlashCard> cardDeck;
	Hash<int, FlashCard> cardDeck;
	int index = 1, rand1 = 0, rand2 = 0, rand3 = 0;
	sf::RenderWindow window;

	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(600.0f * screenScalingFactor, 600.0f * screenScalingFactor), "TESTLY Flashcards", sf::Style::Close | sf::Style::Titlebar);

	sf::Font font;
	font.loadFromFile("src/Font/JetBrainsMono-VariableFont_wght.ttf");

	// aesthetics (backgrounds, textures, sprites; ideas welcome)
	sf::Sprite backgroundSp;
	sf::Texture backgroundTr;
	if (!backgroundTr.loadFromFile("src/Textures/background.png"))
		std::cout << "Couldn't load background.png..." << std::endl;

	backgroundTr.setSmooth(true);
	backgroundTr.setRepeated(false);

	backgroundSp.setOrigin({ static_cast<float>(window.getSize().x / 4), static_cast<float>(window.getSize().y / 4) });
	backgroundSp.setPosition({ static_cast<float>(window.getSize().x / 6 - 50), static_cast<float>(window.getSize().y / 6) });
	backgroundSp.setTexture(backgroundTr);

	// directions implementation
	sf::Text directions, directionsTitle;
	directionsTitle.setFont(font);
	directionsTitle.setCharacterSize(16);
	directionsTitle.setFillColor(sf::Color::White);
	directionsTitle.setOrigin(sf::Vector2f(directions.getCharacterSize() / 2, directions.getCharacterSize() / 2));
	directionsTitle.setPosition({ 100, 75 });
	directionsTitle.setString("Directions:");
	directions.setFont(font);
	directions.setCharacterSize(16);
	directions.setFillColor(sf::Color::White);
	directions.setOrigin(sf::Vector2f(directions.getCharacterSize() / 2, directions.getCharacterSize() / 2));
	directions.setPosition({ 93, 100 });
	directions.setString("You can add and delete flash cards from\n"
						 "the menu, and you can also choose to play\n"
						 "a match game. In Match, we'll associate your\n"
						 "terms and definitions with a flashcard. You\n"
						 "can click on the question card, drag it, and\n"
						 "hover it over the corresponding answer card\n"
						 "for points/time! Press E to go back to menu...\n"
						 "(that works for every selection from the menu)");

	// add card implementation
	TextBox tbox1(16, sf::Color::Black, false);
	tbox1.setFont(font);
	tbox1.setpos({ 100, 100 });
	tbox1.setLimit(true, 30);

	TextBox tbox2(16, sf::Color::Black, false);
	tbox2.setFont(font);
	tbox2.setpos({ 100, 200 });
	tbox2.setLimit(true, 300);

	sf::Text t1, t2, cap;
	t1.setFont(font);
	t1.setCharacterSize(20);
	t1.setString("Add Term");
	sf::RectangleShape rect1(sf::Vector2f(400, 40));

	t2.setFont(font);
	t2.setCharacterSize(20);
	t2.setString("Definition");
	sf::RectangleShape rect2(sf::Vector2f(400, 200));

	cap.setFont(font);
	cap.setCharacterSize(18);
	cap.setString("/300");
	cap.setFillColor(sf::Color(180, 180, 180, 255));

	t1.setPosition({ 100, 75 });
	t2.setPosition({ 100, 175 });
	cap.setPosition({ 400, 410 });

	rect1.setPosition({ 100, 100 });
	rect2.setPosition({ 100, 200 });
	rect1.setOutlineColor(sf::Color::Black);
	rect2.setOutlineColor(sf::Color::Black);

	sf::RectangleShape nextBox(sf::Vector2f(75, 30));
	sf::RectangleShape doneBox(sf::Vector2f(75, 30));
	sf::RectangleShape backBox(sf::Vector2f(75, 30));

	sf::Text nextText, doneText, backText;
	nextText.setFont(font);
	doneText.setFont(font);
	backText.setFont(font);
	nextText.setCharacterSize(18);
	doneText.setCharacterSize(18);
	backText.setCharacterSize(18);
	nextText.setString("NEXT");
	doneText.setString("DONE");
	backText.setString("BACK");
	nextText.setFillColor(sf::Color::Black);
	doneText.setFillColor(sf::Color::Black);
	backText.setFillColor(sf::Color::Black);

	//position the button boxes and texts
	nextBox.setPosition({ 360, 450 });
	nextText.setPosition({ 375, 455 });

	doneBox.setPosition({ 450, 450 });
	doneText.setPosition({ 465, 455 });

	backBox.setPosition({ 100, 300 });
	backText.setPosition({ 115, 305 });

	//deleteC implementation
	TextBox tbox3(16, sf::Color::Black, false);
	tbox3.setFont(font);
	tbox3.setpos({ 100, 210 });
	tbox3.setLimit(true, 30);

	sf::Text t3;
	t3.setFont(font);
	t3.setCharacterSize(20);
	t3.setString("Delete Term");
	t3.setPosition({ 100, 75 });
	t3.setFillColor(sf::Color::White);

	sf::RectangleShape rect3(sf::Vector2f(400, 40));
	rect3.setPosition({ 100, 200 });
	rect3.setOutlineColor(sf::Color::Black);

	// menu implementation
	sf::RectangleShape menu(sf::Vector2f(600, 600));
	menu.setOrigin(sf::Vector2f(menu.getSize().x / 2, menu.getSize().y / 2));
	menu.setFillColor(sf::Color::Black);
	menu.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	sf::Text menuT, option1, option2, option3, option4, instr;
	menuT.setFont(font);
	option1.setFont(font);
	option2.setFont(font);
	option3.setFont(font);
	option4.setFont(font);
	instr.setFont(font);
	menuT.setCharacterSize(36);
	option1.setCharacterSize(30);
	option2.setCharacterSize(30);
	option3.setCharacterSize(30);
	option4.setCharacterSize(30);
	instr.setCharacterSize(26);
	menuT.setFillColor(sf::Color::White);
	option1.setFillColor(sf::Color::White);
	option2.setFillColor(sf::Color::White);
	option3.setFillColor(sf::Color::White);
	option4.setFillColor(sf::Color::White);
	instr.setFillColor(sf::Color::White);
	menuT.setOrigin(sf::Vector2f(menuT.getCharacterSize() / 2, menuT.getCharacterSize() / 2));
	option1.setOrigin(sf::Vector2f(option1.getCharacterSize() / 2, option1.getCharacterSize() / 2));
	option2.setOrigin(sf::Vector2f(option2.getCharacterSize() / 2, option2.getCharacterSize() / 2));
	option3.setOrigin(sf::Vector2f(option3.getCharacterSize() / 2, option3.getCharacterSize() / 2));
	option4.setOrigin(sf::Vector2f(option4.getCharacterSize() / 2, option4.getCharacterSize() / 2));
	instr.setOrigin(sf::Vector2f(instr.getCharacterSize() / 2, instr.getCharacterSize() / 2));
	menuT.setString("Welcome To Testly!");
	option1.setString("A) Add FlashCard");
	option2.setString("B) Delete FlashCard");
	option3.setString("C) Match!");
	option4.setString("D) Directions");
	instr.setString("Select an option by pressing the corresponding letter!");

	float verticalReference = window.getSize().y / 4;
	menuT.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference));
	instr.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference + 50));
	option1.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 2));
	option2.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 2 + 35));
	option3.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 2 + (35 * 2)));
	option4.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 2 + (35 * 3)));

	// match implementation

	sf::Text matchTitle, note, goodJob;
	matchTitle.setFont(font);
	note.setFont(font);
	goodJob.setFont(font);
	matchTitle.setString("Testly");
	note.setString("Press E to exit!");
	goodJob.setString("Good Job! Press E for menu!");
	matchTitle.setCharacterSize(16);
	note.setCharacterSize(12);
	goodJob.setCharacterSize(30);
	matchTitle.setOrigin(sf::Vector2f(matchTitle.getCharacterSize(), matchTitle.getCharacterSize() / 2));
	note.setOrigin(sf::Vector2f(note.getCharacterSize(), note.getCharacterSize() / 2));
	goodJob.setOrigin(sf::Vector2f(note.getCharacterSize(), goodJob.getCharacterSize() / 2));
	matchTitle.setFillColor(sf::Color::White);
	note.setFillColor(sf::Color::White);
	goodJob.setFillColor(sf::Color::White);
	matchTitle.setPosition(sf::Vector2f(window.getSize().x / 3, 20));
	note.setPosition(sf::Vector2f(window.getSize().x / 3 - 20, 40));
	goodJob.setPosition(sf::Vector2f(75, window.getSize().y / 2));

	bool menuPending = true, addC = false, match = false, deleteC = false, dir = false, firstRun = true; // extra booleans to control the flow of the window relative to its internal relations
																										 // (deleteC and dir not used yet, so they cause errors if not commented out)
	sf::Vector2f randomQP, randomAP;
	FlashCard temp1, temp2, temp3, temp4, temp5;
	std::vector<std::string> questions;

	// changed focuses:

	// 1. need to implement review and exit menu options
	// 2. need to implement delete card success message so the user knows it worked
	// 3. expand match so user can decide to play again (maybe also add menu option to load terms and definitions from file)
	// 3b. if we add a load option, we should have a box in add card thats like "save to file" or something

	// after these things are improved, we can start thinking about aesthetics, we could also add an exit option in menu

	while (window.isOpen())
	{
		sf::Event event;
		if (menuPending) // i added this conditional because when the event transitions to add flashcard, console prints "That is not a valid option ... ", this way the event wont be polled once menuPending = false
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
					default:
						//std::cout << "That is not a valid option, please try again or exit the window to close\n";
						break;
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::KeyPressed:
						if (event.key.code == sf::Keyboard::A) // transition to add card
						{
							menuPending = false;
							addC = true;
							std::cout << "You pressed A!\n";
						}
						else if (event.key.code == sf::Keyboard::B) // transition to delete card
						{
							menuPending = false;
							deleteC = true;
							std::cout << "You pressed B!\n";
						}
						else if (event.key.code == sf::Keyboard::C) // transition to Match game
						{
							menuPending = false;
							match = true;
							std::cout << "You pressed C!\n";
						}
						else if (event.key.code == sf::Keyboard::D) // transition to Match game
						{
							menuPending = false;
							dir = true;
							std::cout << "You pressed D!\n";
						}
						else
						{
							std::cout << "That is not a valid option, please try again or exit the window to close\n";
						}
				}
			}
		}
		// directions implementation
		if (dir)
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
					default:
						break;
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::KeyPressed:
						if (event.key.code == sf::Keyboard::E)
						{
							dir = false;
							menuPending = true;
						}
						break;
				}
			}
		}

		// addCard implementation
		if (addC && sf::Mouse::isButtonPressed(sf::Mouse::Left)) // addC added to conditional so this only runs when A is pressed
		{
			if (rect1.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if top box is being clicked
			{
				tbox2.setSelected(false);
				tbox1.setSelected(true);
			}

			else if (rect2.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if bottom text box is being clicked
			{
				tbox1.setSelected(false);
				tbox2.setSelected(true);
			}

			else if (nextBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if next button is being clicked
			{
				//add current card to map
				//clear text box objects and what is being drawn for a new card
				FlashCard temp;
				temp.setCardSize(sf::Vector2f(100.0f, 50.0f));
				temp.setCardOrigin({ 50.0f, 25.0f });
				temp.setA(tbox2.getText());
				temp.setQ(tbox1.getText());
				if (!temp.isEmpty())
				{
					temp.setDefault(font);
					cardDeck.insert(index++, temp);
					tbox1.clear();
					tbox2.clear();
				}
				else
				{
					std::cout << "Cannot save blank textbox, please fill in both textboxes and try again" << std::endl;
				}
			}

			else if (doneBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if done button is being clicked
			{
				FlashCard temp;
				temp.setCardSize(sf::Vector2f(100.0f, 50.0f));
				temp.setCardOrigin({ 50.0f, 25.0f });
				temp.setA(tbox2.getText());
				temp.setQ(tbox1.getText());
				if (!temp.isEmpty())
				{
					temp.setDefault(font);
					cardDeck.insert(index++, temp);
					tbox1.clear();
					tbox2.clear();
				}
				addC = false;
				menuPending = true;
			}

			else
			{
				tbox1.setSelected(false);
				tbox2.setSelected(false);
			}
		}
		else if (addC && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			tbox1.setSelected(false);
			tbox2.setSelected(false);
		}
		if (!menuPending && addC)
		{
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
		}

		// delete card implementation
		if (deleteC && sf::Mouse::isButtonPressed(sf::Mouse::Left)) // addC added to conditional so this only runs when A is pressed
		{
			if (rect3.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
				tbox3.setSelected(true);
			else if (backBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			{
				menuPending = true;
				deleteC = false;
			}

			else
				tbox3.setSelected(false);
		}
		else if (deleteC && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			tbox3.setSelected(false);
		if (!menuPending && deleteC)
		{
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
						tbox3.typedOn(event);
						break;
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				int i = 0;
				std::string term = tbox3.getText();
				if (term != "")
				{
					for (std::unordered_map<int, FlashCard>::iterator it = cardDeck.sut.begin(); it != cardDeck.sut.end(); it++)
					{
						if (it->second.getQ() == term)
							i = it->first;
					}
					if (i != 0)
						cardDeck.sut.erase(i);
					else
						std::cout << "Could not find term!\n";

					tbox3.clear();
				}
			}
		}

		// match implementation
		if (match)
		{
			while (window.pollEvent(event))
			{
				switch (event.type)
				{
					default:
						break;
					case sf::Event::Closed:
						window.close();
						break;
					case sf::Event::MouseButtonPressed:
						matchGameLogistics(cardDeck.sut[rand1], window, note, matchTitle, font, event, cardDeck, rand2, rand3);
						matchGameLogistics(cardDeck.sut[rand2], window, note, matchTitle, font, event, cardDeck, rand1, rand3);
						matchGameLogistics(cardDeck.sut[rand3], window, note, matchTitle, font, event, cardDeck, rand1, rand2);
						if (checkOpacity(cardDeck.sut[rand1], cardDeck.sut[rand2], cardDeck.sut[rand3])) // if these are not all transparent, checkOpacity returns false
						{
							window.clear();
							window.draw(matchTitle);
							window.draw(goodJob);
							window.display();
							sleep(2);
						}

						break;
					case sf::Event::KeyPressed:
						if (event.key.code == sf::Keyboard::E)
						{
							match = false;
							menuPending = true;
							setDefault(cardDeck, font);
							firstRun = true;
						}
						break;
				}
			}
		}

		window.clear();
		if (menuPending) // display menu
		{
			window.draw(backgroundSp);
			window.draw(menuT);
			window.draw(option1);
			window.draw(option2);
			window.draw(option3);
			window.draw(option4);
		}

		if (!menuPending && dir)
		{
			window.draw(backgroundSp);
			window.draw(directionsTitle);
			window.draw(directions);
		}

		if (!menuPending && deleteC) // display delete card
		{
			window.draw(backgroundSp);
			window.draw(rect3);
			window.draw(t3);
			window.draw(backBox);
			window.draw(backText);
			tbox3.drawTo(window);
		}

		if (!menuPending && addC) // display add card
		{
			window.draw(backgroundSp); // menu is just a blank black rectangle that fits the window
			window.draw(rect1);
			window.draw(rect2);
			window.draw(t1);
			window.draw(t2);

			//buttons at bottom
			window.draw(nextBox);
			window.draw(doneBox);
			window.draw(nextText);
			window.draw(doneText);

			std::stringstream limit;
			limit << tbox2.getText().length() << "/301";
			cap.setString(limit.str());

			window.draw(cap);

			float x1 = rect1.getSize().x + rect1.getPosition().x;
			float x2 = rect2.getSize().x + rect2.getPosition().x;

			tbox1.wrap(x1);
			tbox2.wrap(x2);

			tbox1.drawTo(window);
			tbox2.drawTo(window);
		}

		if (!menuPending && match)
		{
			window.draw(menu); // black shape that covers window
							   // window.draw(newFlashCard.getCardQ());
							   // window.draw(newFlashCard.getCardA());
			window.draw(note);
			if (firstRun)
			{
				rand1 = rand() % (index - 1) + 1;
				rand2 = rand() % (index - 1) + 1;
				rand3 = rand() % (index - 1) + 1;
				while (rand1 == rand2)
					rand2 = rand() % (index - 1) + 1;
				while (rand1 == rand3 || rand2 == rand3)
					rand3 = rand() % (index - 1) + 1;

				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand2], font);
				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand3], font);
				checkCollision(cardDeck.sut[rand2], cardDeck.sut[rand3], font);

				firstRun = false;
			}
			cardDeck.sut[rand1].draw(window, font);
			cardDeck.sut[rand2].draw(window, font);
			cardDeck.sut[rand3].draw(window, font);
			window.draw(matchTitle);
		}

		window.display();
	}

	return 0;
}