#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
class Shape {
public:
	size_t width;
	size_t height;
	std::vector< std::vector<bool> > mass;
	sf::Texture texture;
	int type_of_texture;
public:
	void print() {
		for( auto vect: mass ) {
			for( auto piece:vect ) {
				std::cout << piece;
			}
			std::cout <<"\n";
		}
		std::cout << "\n";
	}
	Shape( size_t x, size_t y, int type, bool value = 0 ): mass(x, std::vector<bool>(y, value)), height(x), width(y), type_of_texture(type) {
	}
	void rotate() {
		std::vector< std::vector<bool> > vect(width, std::vector<bool>(height));
		for( int i = 0; i < height; ++i ) {
			for( int j = 0; j < width; ++j ) {
				vect[width - j - 1][i] = mass[i][j];	
			}
		}
		std::swap( width, height );
		mass = vect;
	}
	std::vector<bool>& operator[]( int i ) {
		return mass[i];
	}
	const std::vector<bool>& operator[]( int i ) const {
		return mass[i];
	}
	
};
// *
// *
//**
class J: public Shape {
public:
	J(): Shape(2,3,0,1) {
		texture.loadFromFile("graphics.png", sf::IntRect(0,0,18,18));
		mass[0][1] = 0;
		mass[0][2] = 0;
	}
};
// *
//***
class Piedestal: public Shape {
public:
	Piedestal(): Shape( 2, 3, 1, 1 ) {
		texture.loadFromFile("graphics.png", sf::IntRect(18,0,18,18));
		mass[0][0] = 0;
		mass[0][2] = 0;
	}
};
//**
// **
class Snake: public Shape {
public:
	Snake(): Shape(2,3,2,1) {
		texture.loadFromFile("graphics.png", sf::IntRect(36,0,18,18));
		mass[0][2] = 0;
		mass[1][0] = 0;
	}
};
// **
//**
class AntiSnake : public Shape {
public:
	AntiSnake(): Shape(2,3,3,1) {
		texture.loadFromFile("graphics.png", sf::IntRect(54,0,18,18));	
		mass[0][0] = 0;
		mass[1][2] = 0;
	}
};
//**
//**
class Square: public Shape {
public:
	Square(): Shape(2,2,4,1) {
		texture.loadFromFile("graphics.png", sf::IntRect(72,0,18,18));		
	}
};
//****
class Rectangle: public Shape {
public:
	Rectangle(): Shape(1, 4, 5, 1) {
		texture.loadFromFile("graphics.png", sf::IntRect(90,0,18,18));
	}
};
//*
//*
//**
class L: public Shape {
public:
	L(): Shape(2,3,6,1) {
		texture.loadFromFile("graphics.png", sf::IntRect(108,0,18,18));
		mass[0][0] = 0;
		mass[0][1] = 0;
	}
};
