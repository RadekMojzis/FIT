#pragma once
#include "stack_of_cards.hpp"
#include "card.hpp"

class Action{
	private:
		Stack_of_cards &from;
		Stack_of_cards &to;
		Card &what;
	public:
		bool execute();
		void reverse();
};