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

// ------------------CollisionGrid Functions-------------------------
CollisionGrid::CollisionGrid()
{

}

CollisionGrid::~CollisionGrid()
{

}

// -------------------Collidable Functions---------------------------
vector<Collidable*> Collidable::collideVec = {};

Collidable::Collidable(const float height, const float width) : _height(height), _width(width)
{
    _position.x1 = this->getPosition().x;
    _position.x2 = this->getPosition().x + (float)getWidth();
    _position.y1 = this->getPosition().y;
    _position.y2 = this->getPosition().y + (float)getHeight();
}

Collidable::Collidable(const float x, const float y, const float height, const float width) :
    _height(height), _width(width)
{
    this->setPosition(x, y);
    _position.x1 = this->getPosition().x;
    _position.x2 = this->getPosition().x + (float)getWidth();
    _position.y1 = this->getPosition().y;
    _position.y2 = this->getPosition().y + (float)getHeight();
}

Collidable::~Collidable ()
{

}

void Collidable::setGrid(const float x1, const float x2, const float y1, const float y2)
{
    _position.x1 = x1;
    _position.x2 = x2;
    _position.y1 = y1;
    _position.y2 = y2;
}

void Collidable::updateGrid(const float x, const float y)
{
    _position.x1 = this->getGrid().x1 + x;
    _position.x2 = this->getGrid().x2 + x;
    _position.y1 = this->getGrid().y1 + y;
    _position.y2 = this->getGrid().y2 + y;
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
    this->setGrid(0,this->getLocalBounds().width,0,this->getLocalBounds().height);

    collideVec.push_back(this);
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

    this->setGrid(x,x+this->getLocalBounds().width,y,y+this->getLocalBounds().height);

    collideVec.push_back(this);
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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        moveFactor = 100; //sprinting
    }


    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // stop movement if left and right
    {
        setxVel(0);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // move left
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
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //move right
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

    if (numJumps == 2 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // jump
    {
        if (getyVel() != 0) { numJumps--; }
        numJumps--;
        setyVel(-500);
    }

    if (numJumps == 1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        secondJump = true;
    }

    if (numJumps == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && secondJump)
    {
        secondJump = false;
        numJumps--;
        setyVel(-500);
    }

    else
    {
        setyVel(getyVel()+g*timeInc);
    }

    if (collideY(getyVel()*timeInc))
    {
        numJumps = 2;
        secondJump = false;
        setyVel(0);
        //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    }

    if (collideX(getxVel()*timeInc))
    {
        setxVel(0);
    }

    this->sf::Sprite::move(getxVel()*timeInc,getyVel()*timeInc);
    this->updateGrid(getxVel()*timeInc,getyVel()*timeInc);

    //std::cout << "Guy: x1: " << this->getGrid().x1 << ", x2: " << this->getGrid().x2 << ", y1: " << this->getGrid().y1 << ", y2: " << this->getGrid().y2 << std::endl;

    time = Clock::clock.restart();
}

bool Character::collideCheck()
{
    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }

        if ((this->getGrid().x2 >= object->getGrid().x1 && this->getGrid().x2 <= object->getGrid().x2)
            ||
            (this->getGrid().x1 >= object->getGrid().x1 && this->getGrid().x1 <= object->getGrid().x2))
        {
            if((this->getGrid().y1 <= object->getGrid().y2 && this->getGrid().y1 >= object->getGrid().y1)
               ||
               (this->getGrid().y2 <= object->getGrid().y2 && this->getGrid().y2 >= object->getGrid().y1))
            {
                return true;
            }
        }
    }
    return false;
}

bool Character::collideX(float moveVal)
{
    float x1 = this->getGrid().x1 + moveVal;
    float x2 = this->getGrid().x2 + moveVal;
    float y1 = this->getGrid().y1;
    float y2 = this->getGrid().y2;

    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }
        if(
            (
             (x1 >= object->getGrid().x1 && x1 <= object->getGrid().x2)
              ||
             (x2 >= object->getGrid().x1 && x2 <= object->getGrid().x2)
            )
             &&
            (
             (y1 >= object->getGrid().y1 && y1 <= object->getGrid().y2)
             ||
             (y2 >= object->getGrid().y1 && y2 <= object->getGrid().y2)
            )
          )
        {
            return true;
        }
    }
    return false;
}

bool Character::collideY(float moveVal)
{
    float y1 = this->getGrid().y1 + moveVal;
    float y2 = this->getGrid().y2 + moveVal;
    float x1 = this->getGrid().x1;
    float x2 = this->getGrid().x2;

    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }
        if(
            (
              (y1 <= object->getGrid().y2 && y1 >= object->getGrid().y1)
               ||
              (y2 <= object->getGrid().y2 && y2 >= object->getGrid().y1)
            )
             &&
            (
              (x1 >= object->getGrid().x1 && x1 <= object->getGrid().x2)
               ||
              (x2 >= object->getGrid().x1 && x2 <= object->getGrid().x2)
            )
          )
        {
            return true;
        }
    }
    return false;
}

bool Character::collideLeft()
{
    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }

        if ((this->getGrid().x2 >= object->getGrid().x1 && this->getGrid().x2 <= object->getGrid().x2)
            &&
            (this->getGrid().x1 <= object->getGrid().x1 || this->getGrid().x1 >= object->getGrid().x2))
        {
            std::cout << "Collision from Left." << std::endl;
            return true;
        }
    }
    return false;
}

bool Character::collideRight()
{
    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }

        if ((this->getGrid().x2 <= object->getGrid().x1 || this->getGrid().x2 >= object->getGrid().x2)
            &&
            (this->getGrid().x1 >= object->getGrid().x1 && this->getGrid().x1 <= object->getGrid().x2))
        {
            std::cout << "Collision from Right." << std::endl;
            return true;
        }
    }
    return false;
}

bool Character::collideTop()
{
    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }
        if((this->getGrid().y1 <= object->getGrid().y2 && this->getGrid().y1 >= object->getGrid().y1)
           &&
           (this->getGrid().y2 >= object->getGrid().y2 || this->getGrid().y2 <= object->getGrid().y1))
        {
            std::cout << "Collision from Top." << std::endl;
            return true;
        }
    }
    return false;
}

bool Character::collideBottom()
{
    for (auto &object : collideVec)
    {
        if (this == object)
        {
            continue;
        }
        if
        (
         //(this->getGrid().y1 >= object->getGrid().y2 || this->getGrid().y1 <= object->getGrid().y1)
           //&&
         (this->getGrid().y2 >= object->getGrid().y1 && this->getGrid().y2 <= object->getGrid().y1+1)
        )
        {
            std::cout << "Collision from bottom." << std::endl;
            return true;
        }
    }
    return false;
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

// -----------------------Block Functions-----------------------------
Block::Block (const int x, const int y, const int width, const int height) :
Block::Block (const int x, const int y, const int width, const int height, const string condition) :
    Collidable(x, y, height, width)
{
    this->setSize(sf::Vector2f(width, height));
    this->sf::Shape::setPosition(x, y);

    if (condition == "Regular") Condition condition = Condition::REGULAR;
    if (condition == "Death") Condition condition = Condition::DEATH;
    if (condition == "Goal") Condition condition = Condition::GOAL;
    collideVec.push_back(this);
}

Block::~Block()
{

}
