#include "Bullet.hpp"


void Bullet::moveUp() { this->rectangle.move(sf::Vector2f(0, -10)); }

void Bullet::move(float x, float y) { rectangle.move(sf::Vector2f(x, y)); }

bool Bullet::checkCollision() {
    if (this->rectangle.getPosition().y < 0) {
        return true;
    }
    else { return false; }
}

Bullet::Bullet(sf::Vector2f gunPosition) {
    this->rectangle.setSize(sf::Vector2f(2.0f, 10.0f));
    this->rectangle.setPosition(gunPosition);
    //std::cout << "Bullet created with ID: " << this->getID() << std::endl; // Debug statement
}

void Bullet::draw(sf::RenderWindow& window) { window.draw(rectangle); }

void Bullet::update(float dt) { this->move(0, -10 * dt); }
sf::FloatRect Bullet::getBounds() const { return rectangle.getGlobalBounds(); }

sf::Vector2f Bullet::getPosition() const {
    return rectangle.getPosition(); // or shape.getPosition(), depending on your member variable
}

bool Bullet::operator==(const Bullet& other) const {
    return this->getID() == other.getID();
}

void Bullet::markForDeletion() {
    markedForDeletion = true;
}

bool Bullet::isMarkedForDeletion() const {
    return markedForDeletion;
}



Bullet::~Bullet() {}
