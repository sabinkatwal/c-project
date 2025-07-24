// src/Game.cpp
#include "Game.hpp"
#include <iostream> // For debugging output
#include <algorithm> // Required for std::remove_if

Game::Game() :
    window(sf::VideoMode({800, 600}), "Angry Birds Clone"),
    bird(100.0f, 450.0f), // Initial bird position (near slingshot)
    mousePressed(false),
    currentGameState(GameState::MainMenu), // Start in MainMenu state
    pendingLevelReset(false) // Initialize pendingLevelReset
{
    window.setFramerateLimit(60);

    // --- Load static resources for pigs once at game start ---
    Pig::loadResources();
    // -------------------------------------------------------------

    // Load font for menu text
    if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
        std::cerr << "Error loading font. Make sure the path is correct." << std::endl;
    }

    // Setup title text
    titleText.setFont(font);
    titleText.setString("Angry Birds Clone");
    titleText.setCharacterSize(50);
    titleText.setFillColor(sf::Color::White);
    titleText.setOrigin(titleText.getLocalBounds().width / 2, titleText.getLocalBounds().height / 2);
    titleText.setPosition(window.getSize().x / 2, window.getSize().y / 2 - 50);

    // Setup start text
    startText.setFont(font);
    startText.setString("Press SPACE to Start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::Yellow);
    startText.setOrigin(startText.getLocalBounds().width / 2, startText.getLocalBounds().height / 2);
    startText.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 50);
}

// New: Method to play sounds and manage their lifetime
void Game::playSound(const sf::SoundBuffer& buffer, float volume) {
    // Create a new sf::Sound object
    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setVolume(volume);
    sound.play();
    // Add it to the queue. std::move is used for efficiency.
    soundQueue.push_back(std::move(sound));
}

void Game::run() {
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart(); // Get time since last frame
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // Handle input based on current game state
        switch (currentGameState) {
            case GameState::MainMenu:
                handleMainMenuInput(event);
                break;
            case GameState::Playing:
                // Mouse button pressed: Start aiming
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        mousePressed = true;
                        mouseClickPos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    }
                }
                // Mouse button released: Launch bird
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left && mousePressed) {
                        mouseReleasePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                        
                        // Calculate launch velocity based on mouse drag
                        // Dragging back from bird's initial position
                        sf::Vector2f launchVector = mouseClickPos - mouseReleasePos;
                        // Scale velocity (adjust multiplier for desired power)
                        float powerMultiplier = 3.0f; 
                        bird.launch(launchVector.x * powerMultiplier, launchVector.y * powerMultiplier);
                        
                        mousePressed = false;
                    }
                }
                // Keyboard input for resetting (e.g., 'R' key)
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::R) {
                        resetLevel();
                    }
                }
                break;
            case GameState::GameOver:
                // Handle game over screen input (e.g., restart, quit)
                break;
        }
    }
}

void Game::update(sf::Time deltaTime) {
    switch (currentGameState) {
        case GameState::MainMenu:
            updateMainMenu(deltaTime);
            break;
        case GameState::Playing:
            bird.update(deltaTime);
            checkCollisions();

            // Remove stopped sounds from the queue to prevent memory leak
            soundQueue.erase(
                std::remove_if(soundQueue.begin(), soundQueue.end(), [](const sf::Sound& s) {
                    return s.getStatus() == sf::Sound::Stopped;
                }),
                soundQueue.end()
            );

            if (!pendingLevelReset) {
                // Check if all pigs are destroyed
                bool allPigsDestroyed = true;
                for (const auto& pig : pigs) {
                    if (pig.isAlive()) {
                        allPigsDestroyed = false;
                        break;
                    }
                }
                if (allPigsDestroyed) {
                    pendingLevelReset = true;
                    levelResetTimer.restart();
                    std::cout << "All pigs destroyed! Waiting to reset level..." << std::endl;
                }
            } else {
                // Wait 1.2 seconds before resetting level to allow sound to play
                if (levelResetTimer.getElapsedTime().asSeconds() > 1.2f) {
                    std::cout << "Level complete! Resetting now." << std::endl;
                    resetLevel();
                    pendingLevelReset = false;
                }
            }
            break;
        case GameState::GameOver:
            // Update game over screen logic
            break;
    }
}

void Game::render() {
    window.clear(sf::Color(100, 149, 237)); // Sky blue background

    // Render based on current game state
    switch (currentGameState) {
        case GameState::MainMenu:
            drawMainMenu();
            break;
        case GameState::Playing:
        { // <--- NEW: Open brace to create a new scope for variable declarations
            // Draw slingshot base (simple rectangle for now)
            sf::RectangleShape slingshotBase({50.0f, 150.0f});
            slingshotBase.setFillColor(sf::Color(139, 69, 19)); // Brown
            slingshotBase.setPosition(80.0f, 450.0f);
            window.draw(slingshotBase);

            // Draw ground (simple rectangle)
            sf::RectangleShape ground({800.0f, 100.0f});
            ground.setFillColor(sf::Color(100, 70, 0)); // Dark brown
            ground.setPosition(0.0f, 500.0f);
            window.draw(ground);

            // Draw aiming line if mouse is pressed and bird is not flying
            if (mousePressed && !bird.isFlying()) {
                sf::Vertex line[] =
                {
                    sf::Vertex(bird.getPosition(), sf::Color::Black),
                    sf::Vertex(mouseClickPos, sf::Color::Black)
                };
                window.draw(line, 2, sf::Lines);
            }

            bird.draw(window); // Draw the bird

            // Draw all alive pigs
            for (auto& pig : pigs) {
                pig.draw(window);
            }
        } // <--- NEW: Close brace for the scope
            break;
        case GameState::GameOver:
            // Draw game over screen elements
            break;
    }

    window.display();
}

void Game::checkCollisions() {
    if (bird.isFlying()) {
        for (auto& pig : pigs) {
            if (pig.isAlive() && bird.getBounds().intersects(pig.getBounds())) {
                pig.hit(this); // Pass 'this' (the Game object) to pig.hit()
                bird.reset(100.0f, 450.0f); // Reset bird after hitting a pig
                std::cout << "Pig hit!" << std::endl;
                break; // Only hit one pig per launch for simplicity
            }
        }
    }
}

void Game::resetLevel() {
    bird.reset(100.0f, 450.0f); // Reset bird to start position
    pigs.clear(); // Clear existing pigs

    // Add new pigs for the level, spaced out horizontally and slightly above ground
    // Adjusted Y position to ensure pigs are fully visible above ground
    pigs.emplace_back(600.0f, 480.0f); // Pig 1 (was 520.0f, adjusted to 480.0f)
    pigs.emplace_back(650.0f, 480.0f); // Pig 2
    pigs.emplace_back(700.0f, 480.0f); // Pig 3

    std::cout << "Level reset. Destroy the pigs!" << std::endl;
}

// Main Menu specific methods
void Game::handleMainMenuInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Space) {
            currentGameState = GameState::Playing; // Transition to playing state
            resetLevel(); // Initialize game elements for playing
        }
    }
}

void Game::updateMainMenu(sf::Time deltaTime) {
    // No dynamic updates for this simple menu, but could add animations etc.
}

void Game::drawMainMenu() {
    window.clear(sf::Color::Black); // Black background for menu
    window.draw(titleText);
    window.draw(startText);
}
