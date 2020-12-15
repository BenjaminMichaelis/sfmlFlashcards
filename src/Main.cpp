/*******************************************************************************************
* Programmers: Hugo Aponte, Ben Michaelis, Zach Grizwold, Blake Calvin
* GitHub Repo: https://github.com/BenjaminMichaelis/sfmlFlashcards		                   *
* Class: CptS 223, Fall, 2020; 											                   *
* Programming Assignment: PA6 - Testly FlashCards						                   *
* Date: December 12th, 2020																   *
* Description: This program creates a window that allows a user several options:           *
*               Add FlashCard, Delete FlashCard, Match, Review, Directions, and Exit!      *
*              We used a Hash and unordered_map, created our own FlashCard,                *
*             TextBox, and TestGame classes to create this game and fulfill all            *
*            assignment requirements. Match and Review are user-interaction based          *
*           which mimic existing quizzing applications like quizlet. Graphics include      *
*               things such as text wrapping                                               *
********************************************************************************************/

#include "FlashCard.hpp"
#include "Platform/Platform.hpp"
#include "TestGame.hpp"
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
bool checkOpacity(FlashCard& f1, FlashCard& f2, FlashCard& f3, FlashCard& f4, FlashCard& f5);
bool checkOpacity(FlashCard& f1, FlashCard& f2, FlashCard& f3, FlashCard& f4, FlashCard& f5)
{
	if (((!f1.checkOpacity() && !f2.checkOpacity()) && (!f3.checkOpacity() && !f4.checkOpacity())) && !f5.checkOpacity()) // return true if all five flashcards are transparent
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
void matchGameLogistics(FlashCard& flashCard, sf::RenderWindow& window, sf::Text& note, sf::Text& matchTitle, sf::Font& font, sf::Event& event, Hash<int, FlashCard>& cardDeck, int index1, int index2, int index3, int index4);
void matchGameLogistics(FlashCard& flashCard, sf::RenderWindow& window, sf::Text& note, sf::Text& matchTitle, sf::Font& font, sf::Event& event, Hash<int, FlashCard>& cardDeck, int index1, int index2, int index3, int index4)
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
			cardDeck.sut[index3].draw(window, font);
			cardDeck.sut[index4].draw(window, font);
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
				cardDeck.sut[index3].draw(window, font);
				cardDeck.sut[index4].draw(window, font);
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
					cardDeck.sut[index3].draw(window, font);
					cardDeck.sut[index4].draw(window, font);
					window.display();
					sleep(1);
					if (i == 0)
					{
						flashCard.setQColor(sf::Color::White);
						cardDeck.sut[index1].setAColor(sf::Color::White);
						flashCard.moveCard('Q', { -10, 0 });
						cardDeck.sut[index1].moveCard('A', { 10, 0 });
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
					cardDeck.sut[index3].draw(window, font);
					cardDeck.sut[index4].draw(window, font);
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
			if (flashCard.checkCollision(cardDeck.sut[index3])) // check for incorrect answer
			{
				flashCard.setQColor(sf::Color::Red);
				cardDeck.sut[index3].setAColor(sf::Color::Red);
				for (int i = 0; i < 2; i++)
				{
					window.clear();
					window.draw(note);
					window.draw(matchTitle);
					flashCard.draw(window, font);
					cardDeck.sut[index1].draw(window, font);
					cardDeck.sut[index2].draw(window, font);
					cardDeck.sut[index3].draw(window, font);
					cardDeck.sut[index4].draw(window, font);
					window.display();
					sleep(1);
					if (i == 0)
					{
						flashCard.setQColor(sf::Color::White);
						cardDeck.sut[index3].setAColor(sf::Color::White);
						flashCard.moveCard('Q', { -10, 0 });
						cardDeck.sut[index3].moveCard('A', { 10, 0 });
					}
				}
			}
			if (flashCard.checkCollision(cardDeck.sut[index4])) // check for incorrect answer
			{
				flashCard.setQColor(sf::Color::Red);
				cardDeck.sut[index4].setAColor(sf::Color::Red);
				for (int i = 0; i < 2; i++)
				{
					window.clear();
					window.draw(note);
					window.draw(matchTitle);
					flashCard.draw(window, font);
					cardDeck.sut[index1].draw(window, font);
					cardDeck.sut[index2].draw(window, font);
					cardDeck.sut[index3].draw(window, font);
					cardDeck.sut[index4].draw(window, font);
					window.display();
					sleep(1);
					if (i == 0)
					{
						flashCard.setQColor(sf::Color::White);
						cardDeck.sut[index4].setAColor(sf::Color::White);
						flashCard.moveCard('Q', { -10, 0 });
						cardDeck.sut[index4].moveCard('A', { 10, 0 });
					}
				}
			}
		}
	}
}
/*////////////////////////////////////////////////////////

                    MAIN PROGRAM

/////////////////////////////////////////////////////////*/
int main()
{
	util::Platform platform;
	srand((unsigned long)time(NULL));

	Hash<int, FlashCard> cardDeck;
	int index = 1, rand1 = 0, rand2 = 0, rand3 = 0, rand4 = 0, rand5 = 0;
	sf::RenderWindow window;

	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());
	// Use the screenScalingFactor
	window.create(sf::VideoMode(850.0f * screenScalingFactor, 850.0f * screenScalingFactor), "TESTLY Flashcards", sf::Style::Close | sf::Style::Titlebar);

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
						 "hover it over the corresponding answer card.\n"
						 "Press E to go back to menu...\n"
						 "(that works for every selection from the menu except add/delete card)");

	// add card implementation
	TextBox tbox1(16, sf::Color::Black, false);
	tbox1.setFont(font);
	tbox1.setpos({ static_cast<float>(window.getSize().x / 4), 150 });
	tbox1.setLimit(true, 30);

	TextBox tbox2(16, sf::Color::Black, false);
	tbox2.setFont(font);
	tbox2.setpos({ static_cast<float>(window.getSize().x / 4), 250 });
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

	t1.setPosition({ static_cast<float>(window.getSize().x / 4), 125 });
	t2.setPosition({ static_cast<float>(window.getSize().x / 4), 225 });
	cap.setPosition({ static_cast<float>(window.getSize().x / 4 + 320), 465 });

	rect1.setPosition({ static_cast<float>(window.getSize().x / 4), 150 });
	rect2.setPosition({ static_cast<float>(window.getSize().x / 4), 250 });
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
	nextBox.setPosition({ static_cast<float>(window.getSize().x / 4 + 220), 490 });
	nextText.setPosition({ static_cast<float>(window.getSize().x / 4 + 235), 495 });

	doneBox.setPosition({ static_cast<float>(window.getSize().x / 4 + 320), 490 });
	doneText.setPosition({ static_cast<float>(window.getSize().x / 4 + 335), 495 });

	backBox.setPosition({ static_cast<float>(window.getSize().x / 4), 300 });
	backText.setPosition({ static_cast<float>(window.getSize().x / 4 + 15), 305 });

	//deleteC implementation
	TextBox tbox3(16, sf::Color::Black, false);
	tbox3.setFont(font);
	tbox3.setpos({ static_cast<float>(window.getSize().x / 4), 210 });
	tbox3.setLimit(true, 30);

	sf::Text t3, success;
	t3.setFont(font);
	t3.setCharacterSize(20);
	t3.setString("Delete Term");
	t3.setPosition({ static_cast<float>(window.getSize().x / 4), 125 });
	t3.setFillColor(sf::Color::White);
	success.setFont(font);
	success.setCharacterSize(20);
	success.setString("Success!");
	success.setPosition({ static_cast<float>(window.getSize().x / 4), 255 });
	success.setFillColor(sf::Color::White);

	sf::RectangleShape rect3(sf::Vector2f(400, 40));
	rect3.setPosition({ static_cast<float>(window.getSize().x / 4), 200 });
	rect3.setOutlineColor(sf::Color::Black);

	// menu implementation
	sf::RectangleShape menu(sf::Vector2f(600, 600));
	menu.setOrigin(sf::Vector2f(menu.getSize().x / 2, menu.getSize().y / 2));
	menu.setFillColor(sf::Color::Black);
	menu.setPosition(sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2));

	sf::Text menuT, option1, option2, option3, option4, instr, option5, option6, warning, tryAgain;
	menuT.setFont(font);
	option1.setFont(font);
	option2.setFont(font);
	option3.setFont(font);
	option4.setFont(font);
	instr.setFont(font);
	option5.setFont(font);
	option6.setFont(font);
	warning.setFont(font);
	tryAgain.setFont(font);
	menuT.setCharacterSize(40);
	option1.setCharacterSize(30);
	option2.setCharacterSize(30);
	option3.setCharacterSize(30);
	option4.setCharacterSize(30);
	instr.setCharacterSize(26);
	option5.setCharacterSize(30);
	option6.setCharacterSize(30);
	warning.setCharacterSize(15);
	tryAgain.setCharacterSize(20);
	menuT.setFillColor(sf::Color::White);
	option1.setFillColor(sf::Color::White);
	option2.setFillColor(sf::Color::White);
	option3.setFillColor(sf::Color::White);
	option4.setFillColor(sf::Color::White);
	instr.setFillColor(sf::Color::White);
	option5.setFillColor(sf::Color::White);
	option6.setFillColor(sf::Color::White);
	warning.setFillColor(sf::Color::White);
	tryAgain.setFillColor(sf::Color::White);
	menuT.setOrigin(sf::Vector2f(menuT.getCharacterSize() / 2, menuT.getCharacterSize() / 2));
	option1.setOrigin(sf::Vector2f(option1.getCharacterSize() / 2, option1.getCharacterSize() / 2));
	option2.setOrigin(sf::Vector2f(option2.getCharacterSize() / 2, option2.getCharacterSize() / 2));
	option3.setOrigin(sf::Vector2f(option3.getCharacterSize() / 2, option3.getCharacterSize() / 2));
	option4.setOrigin(sf::Vector2f(option4.getCharacterSize() / 2, option4.getCharacterSize() / 2));
	instr.setOrigin(sf::Vector2f(instr.getCharacterSize() / 2, instr.getCharacterSize() / 2));
	option5.setOrigin(sf::Vector2f(option5.getCharacterSize() / 2, option5.getCharacterSize() / 2));
	option6.setOrigin(sf::Vector2f(option6.getCharacterSize() / 2, option6.getCharacterSize() / 2));
	warning.setOrigin(sf::Vector2f(warning.getCharacterSize() / 2, warning.getCharacterSize() / 2));
	tryAgain.setOrigin(sf::Vector2f(tryAgain.getCharacterSize() / 2, tryAgain.getCharacterSize() / 2));
	menuT.setString("Welcome To Testly!");
	option1.setString("A) Add FlashCard");
	option2.setString("B) Delete FlashCard");
	option3.setString("C) Match!");
	option4.setString("D) Directions");
	instr.setString("Select an option by pressing the corresponding letter!");
	option5.setString("E) Review");
	option6.setString("F) Exit");
	warning.setString("Warning: Cannot begin match, review,\nnor delete unless at least 5 cards are added!");
	tryAgain.setString("Try Again!");

	float verticalReference = window.getSize().y / 4;
	menuT.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference));
	instr.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference + 50));
	option1.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5));
	option2.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5 + 35));
	option3.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5 + (35 * 2)));
	option4.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5 + (35 * 3)));
	option5.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5 + (35 * 4)));
	option6.setPosition(sf::Vector2f(window.getSize().x / 4 - 20, verticalReference * 1.5 + (35 * 5)));
	warning.setPosition(sf::Vector2f(window.getSize().x / 4 - 60, verticalReference * 1.5 + (35 * 6) + 15));
	tryAgain.setPosition(sf::Vector2f(window.getSize().x / 4, verticalReference * 1.5 + (35 * 8)));
	// match implementation

	sf::Text matchTitle, note, goodJob, playAgain;
	matchTitle.setFont(font);
	note.setFont(font);
	goodJob.setFont(font);
	playAgain.setFont(font);
	matchTitle.setString("Testly");
	note.setString("Press E to exit!");
	goodJob.setString("Good Job! Press E for menu!\nPress F to play again!");
	playAgain.setString("Press F to play again!");
	matchTitle.setCharacterSize(16);
	note.setCharacterSize(12);
	goodJob.setCharacterSize(30);
	playAgain.setCharacterSize(12);
	matchTitle.setOrigin(sf::Vector2f(matchTitle.getCharacterSize(), matchTitle.getCharacterSize() / 2));
	note.setOrigin(sf::Vector2f(note.getCharacterSize(), note.getCharacterSize() / 2));
	goodJob.setOrigin(sf::Vector2f(note.getCharacterSize(), goodJob.getCharacterSize() / 2));
	playAgain.setOrigin(sf::Vector2f(playAgain.getCharacterSize(), playAgain.getCharacterSize() / 2));
	matchTitle.setFillColor(sf::Color::White);
	note.setFillColor(sf::Color::White);
	goodJob.setFillColor(sf::Color::White);
	playAgain.setFillColor(sf::Color::White);
	matchTitle.setPosition(sf::Vector2f(window.getSize().x / 3, 20));
	note.setPosition(sf::Vector2f(window.getSize().x / 3 - 20, 40));
	goodJob.setPosition(sf::Vector2f(75, window.getSize().y / 2));
	playAgain.setPosition(sf::Vector2f(window.getSize().x / 3 - 20, 60));

	// Review implementation
	sf::RectangleShape reviewCard(sf::Vector2f(500, 300));
	reviewCard.setOrigin(reviewCard.getSize().x / 2, reviewCard.getSize().y / 2);
	reviewCard.setFillColor(sf::Color::White);
	reviewCard.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 100);
	sf::Text reviewText;
	reviewText.setFont(font);
	reviewText.setCharacterSize(18);
	reviewText.setOrigin(reviewText.getCharacterSize() / 2, reviewText.getCharacterSize() / 2);
	reviewText.setFillColor(sf::Color::Black);
	reviewText.setPosition(reviewCard.getPosition().x, reviewCard.getPosition().y);
	FlashCard reviewFlashCard;

	bool menuPending = true,
		 addC = false, match = false, deleteC = false, dir = false, firstRun = true, playAgainB = false, review = false, reviewQ = false, reviewA = true, fiveCardsAdded = false; // extra booleans to control the flow of the window relative to its internal relations
																																												  // (deleteC and dir not used yet, so they cause errors if not commented out)
	sf::Vector2f randomQP, randomAP;
	FlashCard temp1;
	std::vector<std::string> questions;

	// Testing Implementation
	TestGame test { 1, temp1 };

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
							if (cardDeck.sut.size() > 0)
							{
								menuPending = false;
								deleteC = true;
							}
							else
							{
								window.clear();
								window.draw(backgroundSp);
								window.draw(menuT);
								window.draw(option1);
								window.draw(option2);
								window.draw(option3);
								window.draw(option4);
								window.draw(option5);
								window.draw(option6);
								window.draw(warning);
								window.draw(tryAgain);
								window.display();
								sleep(1.5);
							}

							std::cout << "You pressed B!\n";
						}
						else if (event.key.code == sf::Keyboard::C) // transition to Match game
						{
							if (fiveCardsAdded)
							{
								menuPending = false;
								match = true;
							}
							else
							{
								window.clear();
								window.draw(backgroundSp);
								window.draw(menuT);
								window.draw(option1);
								window.draw(option2);
								window.draw(option3);
								window.draw(option4);
								window.draw(option5);
								window.draw(option6);
								window.draw(warning);
								window.draw(tryAgain);
								window.display();
								sleep(1.5);
							}

							std::cout << "You pressed C!\n";
						}
						else if (event.key.code == sf::Keyboard::D) // transition to directions
						{
							menuPending = false;
							dir = true;
							std::cout << "You pressed D!\n";
						}
						else if (event.key.code == sf::Keyboard::E) // transition to review
						{
							if (fiveCardsAdded)
							{
								menuPending = false;
								review = true;
							}
							else
							{
								window.clear();
								window.draw(backgroundSp);
								window.draw(menuT);
								window.draw(option1);
								window.draw(option2);
								window.draw(option3);
								window.draw(option4);
								window.draw(option5);
								window.draw(option6);
								window.draw(warning);
								window.draw(tryAgain);
								window.display();
								sleep(1.5);
							}

							std::cout << "You pressed E!\n";
						}
						else if (event.key.code == sf::Keyboard::F) // close window
						{
							menuPending = false;
							window.close();
							std::cout << "You pressed F!\n";
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
		if (addC)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // addC added to conditional so this only runs when A is pressed
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
					temp.setA(tbox2.getText());
					temp.setQ(tbox1.getText());
					if (!temp.isEmpty())
					{
						temp.setDefault(font);
						cardDeck.insert(index++, temp);
						tbox1.clear();
						tbox2.clear();
						if (cardDeck.sut.size() >= 5)
							fiveCardsAdded = true;
					}
				}

				else if (doneBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if done button is being clicked
				{
					FlashCard temp;
					temp.setA(tbox2.getText());
					temp.setQ(tbox1.getText());
					if (!temp.isEmpty())
					{
						temp.setDefault(font);
						cardDeck.insert(index++, temp);
						tbox1.clear();
						tbox2.clear();
						if (cardDeck.sut.size() >= 5)
							fiveCardsAdded = true;
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
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				tbox1.setSelected(false);
				tbox2.setSelected(false);
			}
			if (!menuPending)
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
		}
		// delete card implementation
		if (deleteC)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) // addC added to conditional so this only runs when A is pressed
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
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				tbox3.setSelected(false);
			if (!menuPending)
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
						{
							cardDeck.sut.erase(i);
							window.clear();
							window.draw(backgroundSp);
							window.draw(rect3);
							window.draw(t3);
							window.draw(backBox);
							window.draw(backText);
							tbox3.drawTo(window);
							window.draw(success);
							window.display();
							sleep(1);
							if (cardDeck.sut.size() < 5)
								fiveCardsAdded = false;
						}

						else
							std::cout << "Could not find term!\n";

						tbox3.clear();
					}
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
						matchGameLogistics(cardDeck.sut[rand1], window, note, matchTitle, font, event, cardDeck, rand2, rand3, rand4, rand5);
						matchGameLogistics(cardDeck.sut[rand2], window, note, matchTitle, font, event, cardDeck, rand1, rand3, rand4, rand5);
						matchGameLogistics(cardDeck.sut[rand3], window, note, matchTitle, font, event, cardDeck, rand1, rand2, rand4, rand5);
						matchGameLogistics(cardDeck.sut[rand4], window, note, matchTitle, font, event, cardDeck, rand1, rand2, rand3, rand5);
						matchGameLogistics(cardDeck.sut[rand5], window, note, matchTitle, font, event, cardDeck, rand1, rand2, rand4, rand3);
						if (checkOpacity(cardDeck.sut[rand1], cardDeck.sut[rand2], cardDeck.sut[rand3], cardDeck.sut[rand4], cardDeck.sut[rand5])) // if these are not all transparent, checkOpacity returns false
						{
							playAgainB = true;
							window.clear();
							window.draw(matchTitle);
							window.draw(goodJob);
							window.draw(playAgain);
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
						else if (event.key.code == sf::Keyboard::F)
						{
							setDefault(cardDeck, font);
							firstRun = true;
							playAgainB = false;
						}
						break;
				}
			}
		}

		// review implementation
		if (review)
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
						if (reviewCard.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
						{
							if (reviewQ)
							{
								reviewText.setString(reviewFlashCard.getQ());
								reviewText.setOrigin(sf::Vector2f(floor(reviewText.getLocalBounds().width / 2), floor(reviewText.getLocalBounds().height / 2)));
							}

							else if (reviewA)
							{
								reviewText.setString(reviewFlashCard.getA());
								reviewText.setOrigin(sf::Vector2f(floor(reviewText.getLocalBounds().width / 2), floor(reviewText.getLocalBounds().height / 2)));
							}
						}
						else if (nextBox.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //check if next button is being clicked
						{
							do
							{
								int random = rand() % (index) + 1;
								reviewFlashCard = cardDeck.sut[random];
							} while (reviewFlashCard.getQ() == "" || reviewFlashCard.getA() == "");

							reviewText.setString(reviewFlashCard.getQ());
							reviewText.setOrigin(sf::Vector2f(floor(reviewText.getLocalBounds().width / 2), floor(reviewText.getLocalBounds().height / 2)));
							reviewQ = false;
							reviewA = true;
						}
						break;
					case sf::Event::KeyPressed:
						if (event.key.code == sf::Keyboard::E)
						{
							review = false;
							menuPending = true;
							reviewQ = false;
							reviewA = true;
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
			window.draw(option5);
			window.draw(option6);
			window.draw(warning);
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
				rand4 = rand() % (index - 1) + 1;
				rand5 = rand() % (index - 1) + 1;
				while (rand1 == rand2)
					rand2 = rand() % (index - 1) + 1;
				while (rand1 == rand3 || rand2 == rand3)
					rand3 = rand() % (index - 1) + 1;
				while ((rand1 == rand4 || rand2 == rand4) || rand3 == rand4)
					rand4 = rand() % (index - 1) + 1;
				while ((rand1 == rand5 || rand2 == rand5) || (rand3 == rand5 || rand4 == rand5))
					rand5 = rand() % (index - 1) + 1;

				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand2], font);
				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand3], font);
				checkCollision(cardDeck.sut[rand2], cardDeck.sut[rand3], font);
				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand4], font);
				checkCollision(cardDeck.sut[rand1], cardDeck.sut[rand5], font);
				checkCollision(cardDeck.sut[rand2], cardDeck.sut[rand4], font);
				checkCollision(cardDeck.sut[rand2], cardDeck.sut[rand5], font);
				checkCollision(cardDeck.sut[rand3], cardDeck.sut[rand4], font);
				checkCollision(cardDeck.sut[rand3], cardDeck.sut[rand5], font);
				checkCollision(cardDeck.sut[rand4], cardDeck.sut[rand5], font);

				firstRun = false;
			}
			if (playAgainB)
			{
				window.draw(playAgain);
			}

			cardDeck.sut[rand1].draw(window, font);
			cardDeck.sut[rand2].draw(window, font);
			cardDeck.sut[rand3].draw(window, font);
			cardDeck.sut[rand4].draw(window, font);
			cardDeck.sut[rand5].draw(window, font);
			window.draw(matchTitle);
		}

		if (!menuPending && review)
		{
			if (firstRun)
			{
				do
				{
					int random = rand() % (index) + 1;
					reviewFlashCard = cardDeck.sut[random];
				} while (reviewFlashCard.getQ() == "" || reviewFlashCard.getA() == "");

				reviewText.setString(reviewFlashCard.getQ());
				reviewText.setOrigin(sf::Vector2f(floor(reviewText.getLocalBounds().width / 2), floor(reviewText.getLocalBounds().height / 2)));
				firstRun = false;
			}
			window.draw(backgroundSp);
			window.draw(reviewCard);
			window.draw(reviewText);
			window.draw(nextBox);
			window.draw(nextText);
			window.draw(note);
		}

		window.display();
	}
	if (cardDeck.sut[0].getTestC())
		test.testCorrectMatch(true);
	else
		std::cout << "TestCorrectMatch(): Inconclusive...";

	if (cardDeck.sut[0].getTestW())
		test.testWrongMatch(true);
	else
		std::cout << "TestCorrectMatch(): Inconclusive...";

	return 0;
}