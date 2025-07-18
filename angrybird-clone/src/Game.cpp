// Game.cpp
#include "Game.hpp"
#include <iostream> // For debugging output

Game::Game() :
    window(sf::VideoMode({800, 600}), "Angry Birds Clone"),
    bird(100.0f, 450.0f), // Initial bird position (near slingshot)
    mousePressed(false)
{
    window.setFramerateLimit(60);
    resetLevel(); // Initialize pigs for the first level
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
    }
}

void Game::update(sf::Time deltaTime) {
    bird.update(deltaTime);
    checkCollisions();

    // Check if all pigs are destroyed
    bool allPigsDestroyed = true;
    for (const auto& pig : pigs) {
        if (pig.isAlive()) {
            allPigsDestroyed = false;
            break;
        }
    }

    if (allPigsDestroyed) {
        std::cout << "All pigs destroyed! Level complete!" << std::endl;
        // Implement level progression or game over screen
        resetLevel(); // For now, just reset the level
    }
}

void Game::render() {
    window.clear(sf::Color(100, 149, 237)); // Sky blue background

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

    window.display();
}

void Game::checkCollisions() {
    if (bird.isFlying()) {
        for (auto& pig : pigs) {
            if (pig.isAlive() && bird.getBounds().intersects(pig.getBounds())) {
                pig.hit(); // Mark pig as hit
                bird.reset(100.0f, 450.0f); // Reset bird after hitting a pig
                std::cout << "Pig hit!" << std::endl;
                break; // Only hit one pig per launch for simplicity
            }
        }
    }
    // Also check bird hitting ground (already handled in Bird::update for now)
}

void Game::resetLevel() {
    bird.reset(100.0f, 450.0f); // Reset bird to start position
    pigs.clear(); // Clear existing pigs
    
    // Add new pigs for the level
    pigs.emplace_back(600.0f, 480.0f); // Pig 1
    pigs.emplace_back(650.0f, 480.0f); // Pig 2
    pigs.emplace_back(700.0f, 480.0f); // Pig 3

    std::cout << "Level reset. Destroy the pigs!" << std::endl;
}
