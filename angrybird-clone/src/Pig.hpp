// src/Pig.hpp
#ifndef PIG_HPP
#define PIG_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game; // Forward declaration of Game class

class Pig {
public:
    Pig(float x, float y);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    bool isAlive() const;
    void hit(Game* game = nullptr);
    bool isSoundPlaying() const;

    // --- Static members for shared resources ---
    static sf::Texture pigTexture;
    static sf::SoundBuffer pigHitSoundBuffer;
    static bool texturesLoaded; // Flag to track if static texture loaded
    static bool soundsLoaded;   // Flag to track if static sound loaded

    static void loadResources(); // Method to load static resources once
    // ------------------------------------------------

private:
    sf::Sprite sprite;
    sf::CircleShape shape; // Keep for collision detection (can be invisible)
    bool alive;
    sf::Sound hitSound; // Each pig still has its own sf::Sound object
};

#endif // PIG_HPP
