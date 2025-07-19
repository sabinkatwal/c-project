// Bird.cpp
#include "Bird.hpp"
#include <iostream>

Bird::Bird(float startX, float startY) :
    initialPosition(startX, startY),
    flying(false)
{
    shape.setRadius(20.0f);
    shape.setFillColor(sf::Color::Red);
    shape.setOrigin(shape.getRadius(), shape.getRadius()); // Center origin for rotation/positioning
    shape.setPosition(initialPosition);
    velocity = {0.0f, 0.0f};
}

void Bird::launch(float initialVelX, float initialVelY) {
    if (!flying) { // Only launch if not already flying
        velocity = {initialVelX, initialVelY};
        flying = true;
    }
}

void Bird::update(sf::Time deltaTime) {
    if (flying) {
        // Apply gravity
        velocity.y += gravity * deltaTime.asSeconds();

        // Update position
        shape.move(velocity * deltaTime.asSeconds());

        // Simple boundary check (e.g., stop at bottom of screen)
        if (shape.getPosition().y + shape.getRadius() > 600.0f) { // Assuming 600px height
            shape.setPosition(shape.getPosition().x, 600.0f - shape.getRadius());
            velocity.y = 0; // Stop vertical movement
            flying = false; // Stop flying when it hits the ground
        }
    }
}

void Bird::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Bird::reset(float startX, float startY) {
    shape.setPosition(startX, startY);
    velocity = {0.0f, 0.0f};
    flying = false;
}

sf::Vector2f Bird::getPosition() const {
    return shape.getPosition();
}

sf::FloatRect Bird::getBounds() const {
    return shape.getGlobalBounds();
}

bool Bird::isFlying() const {
    return flying;
}
