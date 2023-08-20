#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

class Alien: public GameObject
{
public:
    Alien(const sf::Texture &texture, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2, sf::Vector2f position);
    Alien(sf::Vector2f positon);
    Alien();
    void move(float x, float y);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void moveDown();
    bool hitsEdge(const sf::RenderWindow &window) const;
    sf::FloatRect getBounds() const override;
    void setDirectionLeft(bool dirLeft);
    sf::Vector2f getPosition() const;
    bool operator==(const Alien& other) const;
    ~Alien();

private:
    sf::Sprite alienSprite;
    float moveDownSpeed;
    bool directionLeft;
    sf::IntRect octopusTexture1;
    sf::IntRect octopusTexture2;
    int frameCounter;
    static const int animationSpeed = 10; // Adjust to the desired speed
    
};
