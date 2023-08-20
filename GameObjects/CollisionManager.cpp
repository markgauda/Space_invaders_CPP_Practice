#include "CollisionManager.hpp"

// Initialize the static instance variable
CollisionManager* CollisionManager::instance = nullptr;

CollisionManager::CollisionManager(const sf::RenderWindow& window): windowBounds(0, 0, window.getSize().x, window.getSize().y) {}

CollisionManager& CollisionManager::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("CollisionManager not initialized!");
    }
    return *instance;
}

bool CollisionManager::checkCollision(const GameObject &a, const GameObject &b) const {
    return a.getBounds().intersects(b.getBounds());
}

CollisionSide CollisionManager::checkWindowCollision(const GameObject& object) const {
    sf::FloatRect objectBounds = object.getBounds();
    if (objectBounds.left <= 0) return LEFT;
    if (objectBounds.left + objectBounds.width >= windowBounds.width) return RIGHT;
    return NONE;
}

void CollisionManager::initialize(const sf::RenderWindow& window) {
    if (instance == nullptr) {
        instance = new CollisionManager(window);
    }
}