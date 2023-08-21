#pragma once
#include <vector>
#include "Alien.hpp"
#include "CollisionManager.hpp"
#include "Bullet.hpp"
#include <functional>


class AlienManager: public GameObject
{
public:
    AlienManager(int numRows, int numColumns, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2);
    void move(float x, float y) override;
    void update(float dt) override; // Added parameter for bullets
    void draw(sf::RenderWindow& window) override;
    sf::FloatRect getBounds() const override;
    void loadTexture();
    void loadAliens(int numRows, int numColumns, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2);
    CollisionSide checkAliensAtBorder() const;
    void sweepAndPrune(const std::vector<Bullet>& bullets); //Sweep and prune helper function
    void setDeathCallback(std::function<void(int)> callback);
    void alienDied();
    bool isOutOfAliens();
    ~AlienManager();

private:
    std::vector<Alien> alienGroup;
    bool alienHitsEdge(const Alien &alien);
    sf::Texture texture;
    void deleteAlien(Alien* alien);
    std::function<void(int)> deathCallback;
    const float collisionThreshold = 50.0f; // Threshold for sweep and prune
};

namespace AlienTextures {
    const sf::IntRect Octopus1(5, 1, 8, 7);
    const sf::IntRect Octopus2(5, 11, 8, 7);
    const sf::IntRect Bug1(22, 1, 10, 7);
    const sf::IntRect Bug2(22, 11, 10, 7);
}

