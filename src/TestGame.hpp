#include "FlashCard.hpp"
#include "Hash.hpp"
#include <iostream>

class TestGame
{
public:
	Hash<int, FlashCard> _test;
	bool testCorrect;
	bool testWrong;

	TestGame() {};
	TestGame(int index, FlashCard f)
	{
		testEmpty();
		testInsert(index, f);
		testDelete(index);
		testCorrect = true;
		testWrong = true;
	}

	void testEmpty()
	{
		if (_test.sut.empty())
			std::cout << "TestEmpty(): Hash successfully created!\n";
		else
		{
			std::cout << "TestEmpty(): Hash unsuccessful...\n";
		}
	}

	void testInsert(int index, FlashCard f)
	{
		_test.sut[index] = f;
		if (_test.sut.empty())
			std::cout << "TestInsert(): Insert unsuccessful...\n";
		else
		{
			std::cout << "TestInsert(): Insert successful!\n";
		}
	}

	void testDelete(int index)
	{
		_test.remove(index);
		if (_test.sut.empty())
			std::cout << "TestDelete(): Deletion successful!\n";
		else
		{
			std::cout << "TestDelete(): Deletion unsuccessful...\n";
		}
	}

	void testCorrectMatch(bool testC)
	{
		if (testC && testCorrect)
		{
			std::cout << "TestCorrectCollision(): Collision detection between correct successful!\n";
			testCorrect = false;
		}
		else
			std::cout << "TestCorrectCollision(): Collision detection error...\n";
	}

	void testWrongMatch(bool testW)
	{
		if (testW && testWrong)
		{
			std::cout << "TestWrongCollision(): Collision detection between incorrect matches successful!\n";
			testWrong = false;
		}
		else
		{
			std::cout << "TestWrongCollision(): Collision detection between incorrect matches unsuccessful...\n";
		}
	}
};