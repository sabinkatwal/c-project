// // Pig.hpp
// #ifndef PIG_HPP
// #define PIG_HPP

// #include <SFML/Graphics.hpp>

// class Pig {
// public:
//     Pig(float x, float y);

//     void draw(sf::RenderWindow& window);
//     sf::FloatRect getBounds() const;
//     bool isAlive() const;
//     void hit(); // Mark pig as hit/destroyed

// private:
//     sf::CircleShape shape;
//     bool alive;
// };

// #endif // PIG_HPP

//Sound Effect Added
// Pig.hpp
#ifndef PIG_HPP
#define PIG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Pig {
public:
    Pig(float x, float y);

    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isAlive() const;
    void hit(); // Mark pig as hit/destroyed and play sound

private:
    sf::CircleShape shape;
    bool alive;

    sf::SoundBuffer hitBuffer;
    sf::Sound hitSound;
};

#endif // PIG_HPP
