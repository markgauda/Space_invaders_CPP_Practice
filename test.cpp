#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "GameObjects/CollisionManager.hpp"
#include "GameObjects/GameObject.hpp"
#include "GameObjects/Bullet.hpp"
#include "GameObjects/BulletManager.hpp"
#include "GameObjects/Alien.hpp"
#include "GameObjects/AlienManager.hpp"

void increaseScore(int& score) {
  score += 10;
}

int main()
{
    // Set window parameters
    int width = 960;
    int height = 720;

    // Set score
    int score = 0;

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

    window.setFramerateLimit(60);
    
    // Set up managers
    CollisionManager::initialize(window);
    BulletManager& bulletManager = BulletManager::getInstance();
    AlienManager alienManager(4, 5, AlienTextures::Octopus1, AlienTextures::Octopus2);

    // Set Score callback
    alienManager.setDeathCallback(std::bind(increaseScore, std::ref(score)));

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
            alienManager.loadAliens(8, 15, AlienTextures::Octopus1, AlienTextures::Octopus2);
        }


        
        //render
        window.clear();
        window.draw(shape);
        window.draw(scoreText);
        bulletManager.drawBullets(window);
        alienManager.draw(window);
        window.display();
    }

    return 0;
}



