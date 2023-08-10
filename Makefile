all: delete command compile run

delete:
	rm -rf *.o game-normal

command:
	g++ -c main.cpp

compile:
	g++ main.o -o game-normal -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system	

run:
	./game-normal
	