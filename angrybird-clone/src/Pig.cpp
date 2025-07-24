// src/Pig.cpp
#include "Pig.hpp"
#include <iostream>
#include "Game.hpp"

// --- Define static members outside the class ---
sf::Texture Pig::pigTexture;
sf::SoundBuffer Pig::pigHitSoundBuffer;
bool Pig::texturesLoaded = false;
bool Pig::soundsLoaded = false;

void Pig::loadResources() {
    if (!texturesLoaded) {
        if (!pigTexture.loadFromFile("src/pig.png")) { // Assuming pig.png is in src/
            std::cerr << "CRITICAL ERROR: Could not load static pig.png! All pigs will be green circles." << std::endl;
            texturesLoaded = false;
        } else {
            texturesLoaded = true;
            std::cout << "DEBUG: Static pig.png loaded successfully." << std::endl;
        }
    }

    if (!soundsLoaded) {
        if (!pigHitSoundBuffer.loadFromFile("src/pig_hit.wav")) { // Assuming pig_hit.wav is in src/
            std::cerr << "CRITICAL ERROR: Could not load static pig_hit.wav! No pig hit sounds will play." << std::endl;
            soundsLoaded = false;
        } else {
            soundsLoaded = true;
            std::cout << "DEBUG: Static pig_hit.wav loaded successfully." << std::endl;
        }
    }
}
// ----------------------------------------------------

Pig::Pig(float x, float y) :
    alive(true)
{
    // Always initialize shape first as a fallback
    shape.setRadius(15.0f);
    shape.setFillColor(sf::Color::Green); // Default to green circle
    shape.setOrigin(shape.getRadius(), shape.getRadius());
    shape.setPosition(x, y);
    shape.setOutlineThickness(0); // No outline by default

    // Use the static texture if loaded
    if (texturesLoaded) {
        sprite.setTexture(pigTexture);
        sprite.setScale(60.0f / pigTexture.getSize().x, 60.0f / pigTexture.getSize().y);
        sprite.setOrigin(pigTexture.getSize().x / 2.0f, pigTexture.getSize().y / 2.0f); // Center origin
        sprite.setPosition(x, y);

        // Make shape transparent as sprite is now the visual, but keep its position/size aligned
        shape.setRadius(sprite.getGlobalBounds().width / 2.0f);
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color::Transparent); // Make shape invisible
    } else {
        // If static texture failed to load, the shape remains the visible green circle fallback
        // Its position is already set above
    }

    // Use the static sound buffer if loaded
    if (soundsLoaded) {
        hitSound.setBuffer(pigHitSoundBuffer);
        hitSound.setVolume(100); // Set volume to 100
    } else {
        // If static sound failed to load, hitSound will not have a buffer
    }
}

void Pig::draw(sf::RenderWindow& window) {
    if (alive) {
        // Draw the sprite if static texture was successfully loaded, otherwise draw the shape fallback
        if (texturesLoaded) {
            window.draw(sprite);
        } else {
            window.draw(shape); // Draw the green circle fallback
        }
    }
}

sf::FloatRect Pig::getBounds() const {
    // Return bounds of the shape for collision detection, as shape is consistently sized for collision
    return shape.getGlobalBounds();
}

bool Pig::isAlive() const {
    return alive;
}

bool Pig::isSoundPlaying() const {
    return hitSound.getStatus() == sf::Sound::Playing;
}

void Pig::hit(Game* game) {
    if (alive) {
        alive = false;
        // Play sound ONLY if static sound buffer was successfully loaded
        if (soundsLoaded) {
            hitSound.play();
            std::cout << "DEBUG: Pig hit at (" << shape.getPosition().x << ", " << shape.getPosition().y << ") and attempting to play sound directly!" << std::endl;
        } else {
            std::cout << "DEBUG: Pig hit at (" << shape.getPosition().x << ", " << shape.getPosition().y << ") but static sound not loaded!" << std::endl;
        }
    }
}
