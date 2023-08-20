#include "BulletManager.hpp"
#include <iostream>

void BulletManager::addBullet(const sf::Vector2f& position) {
    if (bullets.size() < MaxBullets){
        bullets.push_back(Bullet(position));
        //std::cout << "Bullet added with ID: " << bullets.back().getID() << std::endl; // Debug statement
    }
}

void BulletManager::updateBullets() {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->moveUp();
        if (it->isMarkedForDeletion() || it->checkCollision()) {
            it = bullets.erase(it);
        }
        else {
            ++it;
        }
    }
}

void BulletManager::drawBullets(sf::RenderWindow& window) {
    for(auto& bullet : bullets) {
        bullet.draw(window);
    }
}

void BulletManager::deleteBullet(const Bullet& bullet) {
    bullets.erase(std::remove(bullets.begin(), bullets.end(), bullet), bullets.end());
    //std::cout << "Deleting bullet with ID: " << bullet.getID() << std::endl; // Debug statement
}



void BulletManager::printBulletInfo() const {
    std::cout << "Current Bullets: " << bullets.size() << std::endl;
    for (const Bullet& bullet : bullets) {
        std::cout << "Bullet ID: " << bullet.getID();
        std::cout << " Position: (" << bullet.getPosition().x << ", " << bullet.getPosition().y << ")";
        std::cout << " Bounds: (" << bullet.getBounds().left << ", " << bullet.getBounds().top << ", " << bullet.getBounds().width << ", " << bullet.getBounds().height << ")";
        std::cout << std::endl;
    }
}

const std::vector<Bullet>& BulletManager::getBullets() const {
    return bullets;
}

