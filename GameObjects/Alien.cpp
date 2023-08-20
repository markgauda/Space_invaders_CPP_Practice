#include "Alien.hpp"
#include "stdio.h"


Alien::Alien(const sf::Texture &texture, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2, sf::Vector2f position)
    : GameObject(), octopusTexture1(textureRect1), octopusTexture2(textureRect2), frameCounter(0) // Added initialization
{
    directionLeft = true;
    alienSprite.setTexture(texture);
    alienSprite.setTextureRect(textureRect1);
    alienSprite.setScale(sf::Vector2f(4.0f, 4.0f));
    alienSprite.setPosition(position);
}
Alien::Alien(sf::Vector2f position)
    : GameObject()
    {
    directionLeft = true;
    alienSprite.setColor(sf::Color::Blue);
    alienSprite.setPosition(position);
}
Alien::Alien()
    : GameObject()
    {
    directionLeft = true;
    // Set sprite to default
}
void Alien::move(float x, float y) {
    alienSprite.move(x, y);
}
void Alien::update(float dt) {
    const float moveSpeed = 2.0f; // Adjust as needed
    float moveX = directionLeft ? -moveSpeed : moveSpeed;

    //std::cout << getID() << " has movement speed " << moveSpeed << " and moving left = " << directionLeft << " Next move is (" << moveX * dt <<" , 0)\n";
    alienSprite.move(moveX * dt, 0);
    frameCounter++;
    if (frameCounter % animationSpeed == 0) { // Every 'animationSpeed' frames, switch texture
        sf::IntRect newTextureRect = (frameCounter / animationSpeed) % 2 == 0 ? octopusTexture1 : octopusTexture2;
        alienSprite.setTextureRect(newTextureRect);
    }
}
void Alien::draw(sf::RenderWindow& window) {
    window.draw(alienSprite);
    // Visualize the bounding box
    // sf::FloatRect bounds = alienSprite.getGlobalBounds();
    // sf::RectangleShape shape;
    // shape.setPosition(bounds.left, bounds.top);
    // shape.setSize(sf::Vector2f(bounds.width, bounds.height));
    // shape.setOutlineColor(sf::Color::Red);
    // shape.setOutlineThickness(1);
    // shape.setFillColor(sf::Color::Transparent);
    // window.draw(shape);
}
void Alien::moveDown() {
    const float moveSpeed = 5.0f; // Adjust as needed
    alienSprite.move(0, moveSpeed); // Move down by moveSpeed
}
bool Alien::hitsEdge(const sf::RenderWindow &window) const {
    sf::FloatRect bounds = alienSprite.getGlobalBounds();
    return bounds.left <= 0 || bounds.left + bounds.width >= window.getSize().x;
}
void Alien::setDirectionLeft(bool dirLeft) { directionLeft = dirLeft; }
sf::FloatRect Alien::getBounds() const {
    return alienSprite.getGlobalBounds();
}
sf::Vector2f Alien::getPosition() const {
    return alienSprite.getPosition(); // or shape.getPosition(), depending on your member variable
}

bool Alien::operator==(const Alien& other) const {
    // Compare the properties that determine equality
    return this->getID() == other.getID();
}
Alien::~Alien() {}
