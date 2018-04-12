//  GameBase.cpp

#include "GameBase.hpp"

int Entity::numEntities = 0;

// ---------------------------Entity Functions--------------------------------------
Entity::Entity (const int x, const int y) : _xCoor(x),_yCoor(y) { addEntity(); }

Entity::~Entity() { deleteEntity(); }

void Entity::addEntity() { numEntities++; }
void Entity::deleteEntity() { numEntities--; }

// -------------------------Character Functions--------------------------------------
void makeChar(sf::Sprite &variableName, const int xpos, const int ypos, std::string filepath)
{
    sf::Texture texture;
    if (!texture.loadFromFile(filepath)) {
        return EXIT_FAILURE;
    }
    sf::Sprite variable(texture);
    variableName = variable;
}

Character::Character (const int x, const int y) : Entity::Entity(x,y) { addEntity(); }

Character::~Character() { deleteEntity(); }
