// Pig.cpp
#include "Pig.hpp"

Pig::Pig(float x, float y) :
    alive(true)
{
    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Green);
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(x, y);
}

void Pig::draw(sf::RenderWindow& window) {
    if (alive) {
        window.draw(shape);
    }
}

sf::FloatRect Pig::getBounds() const {
    return shape.getGlobalBounds();
}

bool Pig::isAlive() const {
    return alive;
}

void Pig::hit() {
    alive = false;
    // Optionally change color or add explosion effect here
}
