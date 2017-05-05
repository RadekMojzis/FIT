

#pragma once
#include "card.hpp"
#include <vector>

class Stack_of_cards{
	// you put cards from deck here
	private:
		std::vector<Card> cards;
	public:
		Stack_of_cards(){};
		void draw();
};