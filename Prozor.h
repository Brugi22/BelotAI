#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class Prozor {
public:
	Prozor() {
		prozor.create(sf::VideoMode(1500, 1000), "Bela");
		gotov = false;
	}
	void ocisti();
	void crtaj(sf::Drawable&);
	void prikazi();
	void update();
	~Prozor() {}
	bool jelGotov() {
		return gotov;
	}
	sf::RenderWindow prozor;
private:
	
	bool gotov;
};

