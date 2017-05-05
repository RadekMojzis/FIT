#pragma once
#include "deck.hpp"
#include "flip.hpp"
#include "foundation.hpp"
#include "pile.hpp"
#include "action.hpp"
#include "PPU.hpp"
#include <vector>

class Game{
	private:
		Deck deck;
		Flip flip;
		std::vector<Action> history;
		
		std::vector<Pile> piles;
		std::vector<Foundation> foundations;
		
		PPU printing_processing_unit;
		
		// nejspis bude mit action jako frienda...
	public:
		void draw();
		void undo();
		//?redo();
		void save();
		void load();
		void quit();
};
