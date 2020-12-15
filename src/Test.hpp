#include "FlashCard.hpp"
#include "Hash.hpp"
#include <iostream>

class Test
{
public:
	Hash<int, FlashCard> _test;

	Test(int index, FlashCard f)
	{
		testInsert(index, FlashCard);
		testDelete(index, FlashCard);
	}
}