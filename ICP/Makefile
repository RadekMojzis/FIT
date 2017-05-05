all : solitere clear

SRC=src/
CC = g++ -std=c++11

solitere: main.o program.o game.o card.o action.o stack_of_cards.o flip.o foundation.o deck.o pile.o PPU.o
	$(CC) main.o program.o game.o card.o action.o stack_of_cards.o flip.o foundation.o deck.o pile.o PPU.o -o $@

main.o: $(SRC)main.cpp
	$(CC) $^ -c

program.o: $(SRC)program.cpp
	$(CC) $^ -c

PPU.o: $(SRC)PPU.cpp
	$(CC) $^ -c

game.o: $(SRC)game.cpp
	$(CC) $^ -c

card.o: $(SRC)card.cpp
	$(CC) $^ -c

action.o: $(SRC)action.cpp
	$(CC) $^ -c

stack_of_cards.o: $(SRC)stack_of_cards.cpp
	$(CC) $^ -c
	
flip.o: $(SRC)flip.cpp
	$(CC) $^ -c

foundation.o: $(SRC)foundation.cpp
	$(CC) $^ -c

deck.o: $(SRC)deck.cpp
	$(CC) $^ -c
	
pile.o: $(SRC)pile.cpp
	$(CC) $^ -c
	

clear: 
	rm *.o

run:
	./program

doxygen:
	doxygen doc/Doxyfile	
