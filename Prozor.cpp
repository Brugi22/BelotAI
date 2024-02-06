#include "Prozor.h"
#include <iostream>
#include <SFML/Graphics.hpp>


void Prozor::update() {
	sf::Event event;
	while (prozor.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			gotov = true;
			prozor.close();
		}
	}
}

void Prozor::crtaj(sf::Drawable& d) {
	prozor.draw(d);
}

void Prozor::prikazi() {
	update();
	prozor.display();
}

void Prozor::ocisti() {
	prozor.clear();
}