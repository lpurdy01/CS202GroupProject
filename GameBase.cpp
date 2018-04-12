//  GameBase.cpp

#include "GameBase.hpp"
#include <iostream>

int Entity::numEntities = 0;

// ----------------------Entity Functions----------------------------
Entity::Entity (const int x, const int y) : _xCoor(x),_yCoor(y) { addEntity(); }

Entity::~Entity() { deleteEntity(); }

void Entity::addEntity() { numEntities++; }
void Entity::deleteEntity() { numEntities--; }

// --------------------Character Functions---------------------------

//Character::Character (const int x, const int y) : Entity::Entity(x,y) { addEntity(); }

Character::Character (const int x, const int y, const std::string filepath)
    : Entity::Entity(x,y)
{
    if (!_texture.loadFromFile(filepath)) {
        return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Character::~Character() { deleteEntity(); }
