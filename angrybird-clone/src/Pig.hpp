// Pig.hpp
#ifndef PIG_HPP
#define PIG_HPP

#include <SFML/Graphics.hpp>

class Pig {
public:
    Pig(float x, float y);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isAlive() const;
    void hit(); // Mark pig as hit/destroyed

private:
    sf::CircleShape shape;
    bool alive;
};

#endif // PIG_HPP
