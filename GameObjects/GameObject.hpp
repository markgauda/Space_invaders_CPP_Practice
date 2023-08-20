#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

class GameObject {
private:
    static int nextID; // To keep track of the next available ID
    int id; // Each object's unique ID

public:
    GameObject(); // Constructor that assigns a unique ID

    virtual void move(float x, float y) = 0;
    virtual void update(float dt) = 0; // dt is the number of time steps to update
    virtual void draw(sf::RenderWindow &window) = 0;
    virtual sf::FloatRect getBounds() const = 0;

    int getID() const; // Getter for the ID
    virtual ~GameObject() {} 
};

#endif
