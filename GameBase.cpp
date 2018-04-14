//  GameBase.cpp

#include "GameBase.hpp"
#include <iostream>

int Entity::numEntities = 0;
vector<Character> charList = { };
Clock Clock::clock;

// ----------------------Entity Functions----------------------------
Entity::Entity (const int x, const int y) : _xCoor(x),_yCoor(y) { addEntity(); }

Entity::~Entity() { deleteEntity(); }

void Entity::addEntity() { numEntities++; }
void Entity::deleteEntity() { numEntities--; }

// --------------------Character Functions---------------------------
Character::Character (std::string filepath)
{
    addEntity();
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
    
    //charList.push_back(*this);
}

Character::Character (const int x, const int y, const std::string filepath)
    : Entity::Entity(x,y)
{
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
    this->setPosition(sf::Vector2f(x,y));
    
    //charList.push_back(*this);
}

Character::~Character() { deleteEntity(); }

void Character::setxVel(int xVel) { _xVel = xVel; }
void Character::setyVel(int yVel) { _yVel = yVel; }
int Character::getxVel() { return _xVel; }
int Character::getyVel() { return _yVel; }


void Character::updateChar() {
    static sf::Time time = Clock::clock.getElapsedTime();
    double timeInc = time.asSeconds();
    std::cout << timeInc << std::endl;
    
    int moveValue = 5;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        setxVel(getxVel()-moveValue);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        setxVel(getxVel()+moveValue);
    }
    else { setxVel(0); }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        setyVel(getyVel()-moveValue);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        setyVel(getyVel()+moveValue);
    }
    else { setyVel(0); }
    
    this->move(getxVel()*timeInc,getyVel()*timeInc);
    
    time = Clock::clock.restart();
}

// --------------------Background Functions---------------------------
Background::Background (const std::string filepath) {
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Background::~Background() { deleteEntity(); }
