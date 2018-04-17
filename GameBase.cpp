//  GameBase.cpp

#include "GameBase.hpp"
#include <iostream>

int Entity::numEntities = 0;
vector<Character> charList = { };
Clock Clock::clock;

// ----------------------Entity Functions----------------------------
Entity::Entity (const int x, const int y) : _xPos(x),_yPos(y) { addEntity(); }

Entity::~Entity() { deleteEntity(); }

void Entity::addEntity() { numEntities++; }
void Entity::deleteEntity() { numEntities--; }

void Entity::setxPos(int xPos) { _xPos = xPos; }
void Entity::setyPos(int yPos) { _yPos = yPos; }
int Entity::getxPos() { return _xPos; }
int Entity::getyPos() { return _yPos; }

// --------------------Collision Functions---------------------------
CollisionGrid::CollisionGrid()
{
    
}

CollisionGrid::~CollisionGrid()
{
    
}

// -------------------Collidable Functions---------------------------
Collidable::Collidable() : _height(0), _width(0)
{

}

Collidable::~Collidable ()
{
    
}

void Collidable::updateGrid(const int x1, const int x2, const int y1, const int y2)
{
    
}

CollisionGrid Collidable::getGrid()
{
    return _position;
}

int Collidable::getHeight()
{
    return _height;
}

int Collidable::getWidth()
{
    return _width;
}


// --------------------Character Functions---------------------------
Character::Character (std::string filepath)
{
    addEntity();
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
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

void Character::setxVel(double xVel) { _xVel = xVel; }
void Character::setyVel(double yVel) { _yVel = yVel; }
double Character::getxVel() { return _xVel; }
double Character::getyVel() { return _yVel; }


void Character::updateChar() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    static sf::Time time = Clock::clock.getElapsedTime();
    double timeInc = time.asSeconds();
    //std::cout << 1/timeInc << std::endl;

    double moveValue = 5;
    double moveFactor = 50;
    double g = 980;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        if (getxVel() < -moveValue*moveFactor)
        {
            setxVel(-moveValue*moveFactor);
        }
        else
        {
            setxVel(getxVel()-moveValue);
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        if (getxVel() > moveValue*moveFactor)
        {
            setxVel(moveValue*moveFactor);
        }
        else
        {
            setxVel(getxVel()+moveValue);
        }
    }
    else { setxVel(0); }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        setyVel(-500);
    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//    {
//        setyVel(getyVel()+moveValue);
//    }
    else {
        setyVel(getyVel()+g*timeInc);

        if (this->getPosition().y > desktop.height/1.5 - 150) {
            setyVel(0);
        }
    }

    this->move(getxVel()*timeInc,getyVel()*timeInc);

    time = Clock::clock.restart();
}

void Character::transpose(const int &x, const int &y) {
    setxPos(x+getxPos());
    setyPos(y+getyPos());
    this->setPosition(getxPos(), getyPos());
}

// --------------------Background Functions---------------------------
Background::Background (const std::string filepath) {
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Background::~Background() { deleteEntity(); }
