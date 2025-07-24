// src/Bird.cpp
#include "Bird.hpp"
#include <iostream>

Bird::Bird(float startX, float startY) :
    initialPosition(startX, startY),
    flying(false)
{
    // Always initialize shape first as a fallback
    shape.setRadius(20.0f);
    shape.setFillColor(sf::Color::Red); // Default to red circle
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(initialPosition);
    shape.setOutlineThickness(0); // No outline by default

    // Load bird texture and set up sprite
    if (!texture.loadFromFile("src/bird.png")) {
        std::cerr << "Error: Could not load bird.png! Drawing red circle fallback." << std::endl;
        shape.setOutlineThickness(2); // Add outline for fallback visibility
        shape.setOutlineColor(sf::Color::Black);
    } else {
        sprite.setTexture(texture);
        sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        // Make the bird sprite large and always visible (e.g., 80x80 px)
        sprite.setScale(80.0f / texture.getSize().x, 80.0f / texture.getSize().y);
        sprite.setPosition(initialPosition);

        // Make shape transparent as sprite is now the visual, but keep its position/size aligned
        shape.setRadius(sprite.getGlobalBounds().width / 2.0f);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(initialPosition);
        shape.setFillColor(sf::Color::Transparent);
    }
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

        // Update position of both sprite and shape
        sprite.move(velocity * deltaTime.asSeconds());
        shape.move(velocity * deltaTime.asSeconds());

        // Simple boundary check (e.g., stop at bottom of screen)
        if (shape.getPosition().y + shape.getRadius() > 600.0f) {
            shape.setPosition(shape.getPosition().x, 600.0f - shape.getRadius());
            sprite.setPosition(shape.getPosition()); // Keep sprite and shape in sync
            velocity.y = 0;
            flying = false;
        }
    }
}

void Bird::draw(sf::RenderWindow& window) {
    // Draw the sprite if texture loaded, otherwise draw the shape fallback
    if (texture.getSize().x > 0 && texture.getSize().y > 0) { // Check if texture is valid
        window.draw(sprite);
    } else {
        window.draw(shape); // Draw the red circle fallback
    }
}

void Bird::reset(float startX, float startY) {
    shape.setPosition(startX, startY);
    sprite.setPosition(startX, startY); // Keep sprite and shape in sync
    velocity = {0.0f, 0.0f};
    flying = false;
}

sf::Vector2f Bird::getPosition() const {
    return shape.getPosition(); // Return position of the collision shape
}

sf::FloatRect Bird::getBounds() const {
    return shape.getGlobalBounds(); // Return bounds of the collision shape
}

bool Bird::isFlying() const {
    return flying;
}
