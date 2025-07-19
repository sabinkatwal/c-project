// Game.hpp
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "Bird.hpp"
#include "Pig.hpp"
#include <vector> // For multiple pigs

class Game {
public:
    Game();
    void run();

private:
    sf::RenderWindow window;
    Bird bird;
    std::vector<Pig> pigs; // Use a vector to hold multiple pigs
    sf::Clock clock;

    bool mousePressed; // To track mouse click for launching
    sf::Vector2f mouseClickPos;
    sf::Vector2f mouseReleasePos;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void checkCollisions(); // Checks for bird-pig collisions
    void resetLevel(); // Resets game state for a new round/level
};

#endif // GAME_HPP
