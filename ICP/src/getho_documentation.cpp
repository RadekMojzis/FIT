class program{
	private:
		std::vector<game> games;
	public:
		create_game();
		//add_game
}

// Or PePeU
class PPU{
	private:
		//Cosi s qt... uz to stahuju...
		//prozatim stdout
		std::fstream output;
	public:
		draw_card(Card &card, int x, int y); // card and position
}

// drawing stejnak musi delat nejaky stack of cards..
typedef int Card // 0-51, cislo karty je number/13, barva je number%13
class Card{
	private:
		int id;
		bool face_up;
	public:
		flip();
}

class Action{
	private:
		Stack_of_cards &from;
		Stack_of_cards &to;
		Card &what;
	public:
		execute(Game &game);
		reverse();
}

class Stack_of_cards{ 
	// a master class of all card stacks
	private:
	  std::vector<Card> cards;
	public:
	 draw();
}

class Deck(stack_of_cards){
	// The thing you draw cards from
	private:
	public:
}

class Flip(stack_of_cards){
	// you put cards from deck here
	private:
	public:
}

class Pile(stack_of_cards){
	// you play on these
	private:
	public:
}

class Foundation(stack_of_cards){
	private:
	public:
	// here you want to put all cards
}
