#include "AlienManager.hpp"
#include "Alien.hpp"
#include "stdio.h"
#include "CollisionManager.hpp"
#include "Bullet.hpp"
#include <algorithm>
#include "BulletManager.hpp"

enum class ObjectType { Alien, Bullet };

struct ObjectWrapper {
    float y;
    ObjectType type;
    GameObject* object;
};

AlienManager::AlienManager(int numRows, int numColumns, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2) {
    // Initialize the alienGroup
    loadTexture();
    loadAliens(numRows, numColumns, textureRect1,textureRect2);
}

void AlienManager::move(float x, float y) {
    for (Alien &alien : alienGroup) {
        alien.move(x, y);
    }
}

void AlienManager::update(float dt) {
    // Existing update logic...
    CollisionSide collisionSide = checkAliensAtBorder();
    if (collisionSide != NONE) {
        bool newDirectionLeft = (collisionSide == RIGHT); // Move left if collided with right, and vice versa
        for (Alien &alien : alienGroup) {
            alien.setDirectionLeft(newDirectionLeft);
            alien.moveDown();
        }
    }
    for (Alien &alien : alienGroup) {
        alien.update(dt);
    }

    // Get the singleton BulletManager instance
    BulletManager& bulletManager = BulletManager::getInstance();

    // Get bullets from BulletManager and pass them to the sweepAndPrune method
    const std::vector<Bullet>& bullets = bulletManager.getBullets();
    sweepAndPrune(bullets);
}

void AlienManager::deleteAlien(Alien* alien) {
    alienGroup.erase(std::remove(alienGroup.begin(), alienGroup.end(), *alien), alienGroup.end());
    alienDied();
}

void AlienManager::sweepAndPrune(const std::vector<Bullet>& bullets) {
    BulletManager& bulletManager = BulletManager::getInstance();

    std::vector<ObjectWrapper> objects;

    // Wrap aliens
    for (const Alien& alien : alienGroup) {
        objects.push_back({alien.getBounds().top, ObjectType::Alien, (GameObject*)&alien});
    }

    // Wrap only active bullets
    for (const Bullet& bullet : bullets) {
        if (!bullet.isMarkedForDeletion()) {
            objects.push_back({bullet.getBounds().top, ObjectType::Bullet, (GameObject*)&bullet});
        }
    }

    // Sort by y-position
    std::sort(objects.begin(), objects.end(), [](const ObjectWrapper& a, const ObjectWrapper& b) {
        return a.y < b.y;
    });

    std::vector<Alien*> activeAliens;
    std::vector<Alien*> aliensToDelete;
    std::vector<Bullet> bulletsToDelete;

    // Iterate through sorted objects and detect collisions
    for (auto& obj : objects) {
        if (obj.type == ObjectType::Alien) {
            activeAliens.push_back((Alien*)obj.object);
        } else {
            Bullet& bullet = *(Bullet*)obj.object;
            auto it = activeAliens.begin();
            bool collisionHandled = false;
            while (it != activeAliens.end() && !collisionHandled) {
                Alien* alienPtr = *it;
                sf::FloatRect bulletBounds = bullet.getBounds();
                sf::FloatRect alienBounds = alienPtr->getBounds();

                if (bulletBounds.intersects(alienBounds)) {
                    // Handle collision
                    bulletsToDelete.push_back(bullet);
                    aliensToDelete.push_back(alienPtr);
                    it = activeAliens.erase(it); // Erase and get the next iterator
                    collisionHandled = true;
                } else {
                    ++it;
                }
            }
        }
    }

    // Delete collided bullets and aliens
    for (const Bullet& bullet : bulletsToDelete) {
        bulletManager.deleteBullet(bullet);
    }
    for (Alien* alien : aliensToDelete) {
        deleteAlien(alien);
    }
}

void AlienManager::draw(sf::RenderWindow& window) {
    for (Alien &alien : alienGroup) {
        alien.draw(window);
    }
}

sf::FloatRect AlienManager::getBounds() const {
    //Returns the union bounds of all the aliens
    if (alienGroup.empty()) return sf::FloatRect(0, 0, 0, 0); // Return empty if no aliens

    sf::FloatRect bounds = alienGroup.front().getBounds();
    for (const Alien &alien : alienGroup) {
        sf::FloatRect alienBounds = alien.getBounds();
        bounds.left = std::min(bounds.left, alienBounds.left);
        bounds.top = std::min(bounds.top, alienBounds.top);
        bounds.width = std::max(bounds.left + bounds.width, alienBounds.left + alienBounds.width) - bounds.left;
        bounds.height = std::max(bounds.top + bounds.height, alienBounds.top + alienBounds.height) - bounds.top;
    }

    return bounds;
}

void AlienManager::loadTexture(){
    if (!texture.loadFromFile("GameObjects/Textures.png")) {
        std::cout << "Could not find texture!";
    }
}

void AlienManager::loadAliens(int numRows, int numColumns, const sf::IntRect &textureRect1, const sf::IntRect &textureRect2) {
    alienGroup.clear();
    int distanceBetweenAliens = 50; // You can adjust this value as needed

    for (int row = 0; row < numRows; row++) {
        for (int column = 0; column < numColumns; column++) {
            sf::Vector2f position(column * distanceBetweenAliens, row * distanceBetweenAliens + 100);
            alienGroup.push_back(Alien(texture, textureRect1, textureRect2, position));
        }
    }
}

CollisionSide AlienManager::checkAliensAtBorder() const {
    CollisionManager& collisionManager = CollisionManager::getInstance();
    for (const Alien &alien : alienGroup) {
        CollisionSide side = collisionManager.checkWindowCollision(alien);
        if (side != NONE) return side;
    }
    return NONE;
}

void AlienManager::setDeathCallback(std::function<void(int)> callback) {
    deathCallback = callback;
}
  
void AlienManager::alienDied() {
    if(deathCallback) {
        // Replace this with the alien point value
        deathCallback(25);
    }
}

bool AlienManager::isOutOfAliens(){
    if (alienGroup.size() <= 0){
        return true;
    }
    else return false;
}

AlienManager::~AlienManager() {
    // Free the alienGroup
}
