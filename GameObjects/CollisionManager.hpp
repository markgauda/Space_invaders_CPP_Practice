#ifndef COLLISION_MANAGER_H
#define COLLISION_MANAGER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "GameObject.hpp"

enum CollisionSide {
    LEFT, RIGHT, NONE
};


class CollisionManager {
    public:
        static CollisionManager& getInstance(); // Used to retrieve the instance

        bool checkCollision(const GameObject &a, const GameObject &b) const;

        CollisionSide checkWindowCollision(const GameObject& object) const;

        static void initialize(const sf::RenderWindow& window); // Used to initialize the instance

    private:
        sf::FloatRect windowBounds;
        static CollisionManager* instance;

        CollisionManager(const sf::RenderWindow& window);
};



#endif