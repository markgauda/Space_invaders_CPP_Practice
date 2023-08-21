#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "GameObjects/CollisionManager.hpp"
#include "GameObjects/GameObject.hpp"
#include "GameObjects/Bullet.hpp"
#include "GameObjects/BulletManager.hpp"
#include "GameObjects/Alien.hpp"
#include "GameObjects/AlienManager.hpp"

void increaseScore(int& score, int increaseBy) {
  score += increaseBy;
}

void displayLevel(int level, sf::RenderWindow &window, sf::Text &levelText) {
   levelText.setString("Level: " + std::to_string(level));
   window.draw(levelText);
   std::cout << "Called level display " << std::endl;
}

std::pair<int, int> getNextLevelParameters(int level) {
    // Constants for maximum rows and columns
    const int maxRows = 8;
    const int maxColumns = 17;
    int rows = 1;
    int columns = maxColumns;

    std::cout << "loading level " << level << std::endl;
    // Base case
    if (level <= 1){
        rows = 2;
        // columns = 3;
    }

    if (2 <= level && level < maxColumns){
        // Calculate the number of rows and columns based on the level
        rows = (level % maxRows) + 1;
        // columns = (2 * level % maxColumns) + 1;
        }
    // The max difficulty before things render off screen
    if (maxRows <= level){
        rows = maxRows;
        // columns = maxColumns;
    }
    return {rows, columns};
}

enum GameState {
    Playing,
    GameOver
};


int main()
{
    // Set window parameters
    int width = 960;
    int height = 720;

    // Set score
    int score = 0;
    
    // Set level
    int level = 1;

    //Load Font
    sf::Font font;
    if (!font.loadFromFile("GameObjects/OpenSans-Bold.ttf")) {
        std::cerr << "Error loading font!";
        return -1;
    }
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    
    // Load level text
    sf::Text levelText;
    levelText.setFont(font);
    levelText.setCharacterSize(24);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(10, 40); // Or any position you like
    levelText.setString("Level: " + std::to_string(level));


    // Make battle ship
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("GameObjects/Textures.png")) {
        std::cerr << "Error loading player texture!";
        return -1; // Or handle the error as you prefer
    }

    sf::Sprite shape; // Creating the Sprite object
    shape.setTexture(playerTexture);
    shape.setTextureRect(sf::IntRect(3, 49, 15 - 2, 56 - 48)); // Defining the sub-region of the texture
    shape.setOrigin((15 - 3) / 2.0f, (56 - 49) / 2.0f); // Setting the origin
    shape.setPosition(sf::Vector2(width/2.0f, height/8.0f * 7)); // Setting the position
    shape.setScale(sf::Vector2f(4.0f, 4.0f));

    // Set up window
    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");

    // Set game state
    GameState gameState = Playing;

    // Set up game over text
    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(40); // Set the size as needed
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(window.getSize().x / 2 - gameOverText.getGlobalBounds().width / 2, window.getSize().y / 2 - gameOverText.getGlobalBounds().height / 2);

    window.setFramerateLimit(60);
    
    // Set up managers
    CollisionManager::initialize(window);
    BulletManager& bulletManager = BulletManager::getInstance();
    auto [rows, columns] = getNextLevelParameters(level);
    AlienManager alienManager(rows, columns, AlienTextures::Octopus1, AlienTextures::Octopus2);

    // Set Score callback
    auto increaseScoreCallback = [&score](int increaseByValue){increaseScore(score, increaseByValue);};
    alienManager.setDeathCallback(increaseScoreCallback);

    int speed = 5.0f;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                bulletManager.addBullet(shape.getPosition());
            }
        }

        // Keyboard handling has been moved out of the pollEvent loop.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
            shape.setColor(sf::Color::Red);
            // bulletManager.addBullet(shape.getPosition());
        }
        else{
            shape.setColor(sf::Color::Green);
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && shape.getPosition().x - shape.getTextureRect().width > 0){
            shape.move(-speed, 0); // Move Left
        }
        if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && shape.getPosition().x + shape.getTextureRect().width < width){
            shape.move(speed, 0); // Move Right
        }
        
        //update the states
        bulletManager.updateBullets();
        alienManager.update(1);
        scoreText.setString("Score: " + std::to_string(score));
        if (alienManager.isOutOfAliens()){
            level++;
            levelText.setString("Level: " + std::to_string(level));
            auto [rows, columns] = getNextLevelParameters(level);
            alienManager.loadAliens(rows, columns, AlienTextures::Octopus1, AlienTextures::Octopus2);
        }

        // Check for game over condition
        sf::FloatRect alienBounds = alienManager.getBounds();
        if (alienBounds.top + alienBounds.height >= height / 8.0f * 7) {
            gameState = GameOver;
        }

        while (gameState == GameOver) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            // Draw the game over text
            window.clear();
            window.draw(gameOverText);
            window.display();

            // Listen for Enter key
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                // Restart the game
                level = 1;
                score = 0;
                auto [rows, columns] = getNextLevelParameters(level);
                levelText.setString("Level: " + std::to_string(level));
                alienManager.loadAliens(rows, columns, AlienTextures::Octopus1, AlienTextures::Octopus2);
                gameState = Playing;
            }
        }


        
        //render
        window.clear();
        window.draw(shape);
        window.draw(scoreText);
        window.draw(levelText);
        bulletManager.drawBullets(window);
        alienManager.draw(window);
        window.display();
    }

    return 0;
}
