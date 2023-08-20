#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "Bullet.hpp"

class BulletManager
{
public:
    static BulletManager& getInstance() {
            static BulletManager instance; // Guaranteed to be destroyed, instantiated on first use
            return instance;
        }

    void addBullet(const sf::Vector2f& position);
    void updateBullets();
    void drawBullets(sf::RenderWindow& window);
    void deleteBullet(const Bullet& bullet);
    const std::vector<Bullet>& getBullets() const;
    void printBulletInfo() const; 

    BulletManager(BulletManager const&) = delete; // Prevent copy construction
    void operator=(BulletManager const&) = delete; // Prevent copy assignment
private:
    std::vector<Bullet> bullets;
    BulletManager() {}
    int const MaxBullets = 2;
};
