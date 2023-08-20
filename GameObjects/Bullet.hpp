#pragma once
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

class Bullet: public GameObject
{
public:
    void moveUp();
    void move(float x, float y) override;
    bool checkCollision();
    Bullet(sf::Vector2f gunPosition);
    void draw(sf::RenderWindow& window) override;
    void update(float dt) override;
    sf::FloatRect getBounds() const override;
    sf::Vector2f getPosition() const;
    bool operator==(const Bullet& other) const;
    bool isMarkedForDeletion() const;
    void markForDeletion();
    ~Bullet();
private:
    sf::RectangleShape rectangle;
    bool markedForDeletion = false;
};
