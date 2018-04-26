//  GameBase.cpp

#include "GameBase.hpp"
#include <iostream>

vector<Character> charList = { };
Clock Clock::clock;

// ----------------------Entity Functions----------------------------
Entity::Entity (const int x, const int y) : _xPos(x),_yPos(y) { }

Entity::~Entity() { }

// --------------------Collision Functions---------------------------

CollisionGrid::CollisionGrid()
{

}

CollisionGrid::~CollisionGrid()
{

}

// -------------------Collidable Functions---------------------------
vector<Collidable*> Collidable::collideVec = {};

Collidable::Collidable() : _height(0), _width(0) { }

Collidable::Collidable(const float height, const float width, const Condition condition) :
    _height(height), _width(width), _condition(condition)
{
    _position.x1 = this->getPosition().x;
    _position.x2 = this->getPosition().x + (float)getWidth();
    _position.y1 = this->getPosition().y;
    _position.y2 = this->getPosition().y + (float)getHeight();
}

Collidable::Collidable(const float x, const float y, const float height, const float width, const Condition condition) :
    _height(height), _width(width), _condition(condition)
{
    this->setPosition(x, y);
    _position.x1 = this->getPosition().x;
    _position.x2 = this->getPosition().x + (float)getWidth();
    _position.y1 = this->getPosition().y;
    _position.y2 = this->getPosition().y + (float)getHeight();
}

void Collidable::setCondition(const Condition condition)
{
    _condition = condition;
}

tuple<Collidable::Condition, float, float> Collidable::getCondition()
{
//    if (_condition == Condition::REGULAR)
//        return _condition;
//    else if (_condition == Condition::DEATH)
//        return "Death";
//    else if (_condition == Condition::GOAL)
//        return "Goal";
//
    float x = 0;
    float y = 0;
    if (_condition == Collidable::TELEPORT)
    {
        x = this->getTeleportX();
        y = this->getTeleportY();
    }
    return std::make_tuple(_condition,x,y);
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

void Collidable::setGridCorner(const float x, const float y)
{
    float width = this->getGrid().x2 - this->getGrid().x1;
    float height =this->getGrid().y2 - this->getGrid().y1;
    _position.x1 = x;
    _position.y1 = y;
    _position.x2 = this->getGrid().x1 + width;
    _position.y2 = this->getGrid().y1 + height;
}

CollisionGrid Collidable::getGrid()
{
    return _position;
}

float Collidable::getHeight()
{
    return _height;
}

float Collidable::getWidth()
{
    return _width;
}

void Collidable::setTeleportX(float xCor) { _teleportXCoor = xCor; }
void Collidable::setTeleportY(float yCor) { _teleportYCoor = yCor; }
float Collidable::getTeleportX() { return _teleportXCoor; }
float Collidable::getTeleportY() { return _teleportYCoor; }

// --------------------Character Functions---------------------------
Character::Character (std::string filepath) : Collidable(0, 0, this->getLocalBounds().height,this->getLocalBounds().width)
{
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
    this->setGrid(0,this->getLocalBounds().width,0,this->getLocalBounds().height);

	setIfDead(false);
}

Character::Character (const int x, const int y, const std::string filepath) :
    Entity::Entity(x,y),
    Collidable(x, y, this->getLocalBounds().height,this->getLocalBounds().width)
{
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
    this->sf::Sprite::setPosition(sf::Vector2f(x,y));

    this->setGrid(x,x+this->getLocalBounds().width,y,y+this->getLocalBounds().height);

	setIfDead(false);
}

Character::~Character()
{}

void Character::setxVel(double xVel) { _xVel = xVel; }
void Character::setyVel(double yVel) { _yVel = yVel; }
double Character::getxVel() { return _xVel; }
double Character::getyVel() { return _yVel; }


void Character::updateChar() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    static sf::Time time = Clock::clock.getElapsedTime();
    double timeInc = time.asSeconds();
    //std::cout << 1/timeInc << std::endl;

    double moveValue = 10;
    double jumpVal = 550;
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
        setyVel(-jumpVal);
    }

    if (numJumps == 1 && !sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        secondJump = true;
    }

    if (numJumps == 1 && sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && secondJump)
    {
        secondJump = false;
        numJumps--;
        setyVel(-jumpVal);
    }

    else
    {
        setyVel(getyVel()+g*timeInc);
    }

    if (collideY(getyVel()*timeInc))
    {
        if(standing(getyVel()*timeInc))
        {
            numJumps = 2;
            secondJump = false;
        }
        setyVel(0);
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


void Character::setxPos(int xPos)
{
    this->sf::Sprite::setPosition(xPos, this->getyPos());
    _xPos = xPos;
}
void Character::setyPos(int yPos)
{
    this->sf::Sprite::setPosition(this->getxPos(),yPos);
    _yPos = yPos;
}
int Character::getxPos() { return _xPos; }
int Character::getyPos() { return _yPos; }

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
             ((y1 >= object->getGrid().y1 && y1 <= object->getGrid().y2)
             ||
             (y2 >= object->getGrid().y1 && y2 <= object->getGrid().y2))
             ||
            ((object->getGrid().y1 >= y1 && object->getGrid().y1 <= y2)
             ||
             (object->getGrid().y2 >= y1 && object->getGrid().y2 <= y2)
             )
            )
          )
        {
            this->blockTrigger(object->getCondition());
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
            this->blockTrigger(object->getCondition());
            return true;
        }
    }
    return false;
}

bool Character::standing(float moveVal)
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
             (y2 <= object->getGrid().y2 && y2 >= object->getGrid().y1)
             &&
             (
               (x1 >= object->getGrid().x1 && x1 <= object->getGrid().x2)
                ||
               (x2 >= object->getGrid().x1 && x2 <= object->getGrid().x2)
             )
           )
          )
        {
            return true;
        }
    }
    return false;
}

void Character::blockTrigger(tuple<Collidable::Condition, float, float> condition)
{
    if (get<0>(condition) == Collidable::DEATH)
    {
        this->setIfDead(true);
    }
    else if (get<0>(condition) == Collidable::GOAL)
    {
        this->setIfWin(true);
    }
    else if (get<0>(condition) == Collidable::TELEPORT)
    {
        this->sf::Sprite::setPosition(get<1>(condition), get<2>(condition));
        this->setGridCorner(get<1>(condition), get<2>(condition));
    }
}

void Character::transpose(const int &x, const int &y) {
    setxPos(x+getxPos());
    setyPos(y+getyPos());
    this->sf::Sprite::setPosition(getxPos(), getyPos());
}

bool Character::checkIfDead()
{
	return _ifDead;
}

void Character::setIfDead(bool dead)
{
	_ifDead = dead;
}

bool Character::checkIfWin()
{
    return _ifWin;
}

void Character::setIfWin(bool win)
{
    _ifWin = win;
}

// --------------------Background Functions---------------------------
Background::Background (const std::string filepath) {
    if (!_texture.loadFromFile(filepath)) {
        //return EXIT_FAILURE;
    }
    this->setTexture(_texture);
}

Background::~Background() { }

// -----------------------Block Functions-----------------------------
vector<Block> Block::blockList = {};

Block::Block (const int x, const int y, const int width, const int height, const Condition condition) :
    Collidable(x, y, height, width, condition)
{
    this->setSize(sf::Vector2f(width, height));
    this->sf::Shape::setPosition(x, y);
    this->sf::Shape::setFillColor(sf::Color::Black);
    
    if(condition == Condition::DEATH) { this->sf::Shape::setFillColor(sf::Color::Red); }
    if(condition == Condition::GOAL) { this->sf::Shape::setFillColor(sf::Color::Yellow); }

    collideVec.push_back(this);
}

Block::~Block()
{

}

// --------------------TeleportBlock Functions---------------------------
TeleportBlock::TeleportBlock (const int x, const int y, const int width, const int height, const float xCor, const float yCor)
: Block(x, y, width, height)
{
    this->setTeleportX(xCor);
    this->setTeleportY(yCor);
    this->setCondition(Collidable::TELEPORT);
}
