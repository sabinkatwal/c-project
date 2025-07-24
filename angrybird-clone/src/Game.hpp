// src/Game.hpp
#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp> // Required for sf::Sound and sf::SoundBuffer
#include "Bird.hpp"
#include "Pig.hpp"
#include <vector>
#include <algorithm> // Required for std::remove_if

class Bird; // Forward declaration for Bird
class Pig;  // Forward declaration for Pig

// Define game states
enum class GameState {
    MainMenu,
    Playing,
    GameOver // You can expand this later
};

class Game {
public:
    Game();
    void run();
    void playSound(const sf::SoundBuffer& buffer, float volume); // New: Method to play sounds

private:
    sf::RenderWindow window;
    Bird bird;
    std::vector<Pig> pigs;
    sf::Clock clock;
    GameState currentGameState;

    bool mousePressed;
    sf::Vector2f mouseClickPos;
    sf::Vector2f mouseReleasePos;

    // For Main Menu
    sf::Font font;
    sf::Text titleText;
    sf::Text startText;

    // For sound management
    std::vector<sf::Sound> soundQueue; // To manage sounds that are currently playing

    // For delayed level reset
    bool pendingLevelReset;
    sf::Clock levelResetTimer;

    void processEvents();
    void update(sf::Time deltaTime);
    void render();
    void checkCollisions();
    void resetLevel();

    // Methods for handling main menu
    void handleMainMenuInput(sf::Event& event);
    void updateMainMenu(sf::Time deltaTime);
    void drawMainMenu();
};

#endif // GAME_HPP
