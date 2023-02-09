all: command build run

command:
	g++ -c main.cpp

build:
	g++ main.o -o game-normal -lsfml-graphics -lsfml-window -lsfml-system

run:
	./game-normal