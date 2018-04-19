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
Collidable::Collidable(const float height, const float width) : _height(height), _width(width)
{
    _position.x1 = this->getPosition().x;
    _position.x2 = this->getPosition().x + (float)getWidth();
    _position.y1 = this->getPosition().y;
    _position.y2 = this->getPosition().y + (float)getHeight();
}

Collidable::~Collidable ()
{

}

void Collidable::updateGrid(const float x1, const float x2, const float y1, const float y2)
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
Character::Character (std::string filepath) :
    Collidable(this->getLocalBounds().height,this->getLocalBounds().width)
{
    addEntity();
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Character::Character (const int x, const int y, const std::string filepath) :
    Entity::Entity(x,y),
    Collidable(this->getLocalBounds().height,this->getLocalBounds().width)

{
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
    this->sf::Sprite::setPosition(sf::Vector2f(x,y));

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

    if (oneTap > 0 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        oneTap--;
        setyVel(-500);
    }
//    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
//    {
//        setyVel(getyVel()+moveValue);
//    }
    else {
        setyVel(getyVel()+g*timeInc);

        if (this->sf::Sprite::getPosition().y > desktop.height/1.5 - 150) {
            if(oneTap<65)
                oneTap++;
            setyVel(0);
        }
    }

    this->sf::Sprite::move(getxVel()*timeInc,getyVel()*timeInc);

    time = Clock::clock.restart();
}

void Character::transpose(const int &x, const int &y) {
    setxPos(x+getxPos());
    setyPos(y+getyPos());
    this->sf::Sprite::setPosition(getxPos(), getyPos());
}

// --------------------Background Functions---------------------------
Background::Background (const std::string filepath) {
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Background::~Background() { deleteEntity(); }
