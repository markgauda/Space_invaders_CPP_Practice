#include "GameObject.hpp"

// Initialize the static member nextID with 0
int GameObject::nextID = 0;

GameObject::GameObject() : id(nextID++) {
    // Constructor body, if needed
}

int GameObject::getID() const {
    return id;
}



// Other virtual method implementations can go here, if needed
