// src/Bird.hpp
#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>

class Bird {
public:
    Bird(float startX, float startY);

    void launch(float initialVelX, float initialVelY);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    void reset(float startX, float startY);

    sf::Vector2f getPosition() const;
    sf::FloatRect getBounds() const;
    bool isFlying() const;

private:
    sf::Texture texture;
    sf::Sprite sprite;
    sf::CircleShape shape; // Keep for collision
    sf::Vector2f velocity;
    bool flying;
    sf::Vector2f initialPosition; // To reset the bird
    const float gravity = 980.0f; // Pixels per second squared (simplified)
};

#endif // BIRD_HPP
