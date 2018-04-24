// GameBase.hpp
// File containing data for everything within the game environment
//     -Can be seperated to multiple hpp's / cpp's if we need to later


#ifndef GAMEBASE_HPP_INCLUDED
#define GAMEBASE_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using std::vector;
#include <list>
using std::list;

// -----------------------Clock Class------------------------------
class Clock : public sf::Clock
{
public:
    static Clock clock;
private:
};

// ----------------------Entity Class------------------------------
class Entity
{
public:
    Entity (const int x = 0, const int y = 0);
    ~Entity();

    static void addEntity();
    static void deleteEntity();
    void setxPos(int xPos);
    void setyPos(int yPos);
    sf::Int32 getxPos();
    sf::Int32 getyPos();

private:
    static int numEntities;
    int _xPos;
    int _yPos;
};

// -------------------CollisionGrid Class--------------------------
class CollisionGrid : public sf::Vector2f {
public:
    CollisionGrid();

    CollisionGrid(float x1, float x2, float y1, float y2);

    ~CollisionGrid();
    
    float x1;
    float x2;
    float y1;
    float y2;
};

// ----------------------Collidable Class--------------------------
class Collidable : public sf::Transformable {
public:
    Collidable(const float height, const float width);
    Collidable(const float x, const float y, const float height, const float width);
    ~Collidable();

    void setGrid(const float x1, const float x2, const float y1, const float y2);
    void updateGrid(const float x, const float y);

    CollisionGrid getGrid();

    int getHeight();
    int getWidth();
    
    static vector<Collidable*> collideVec;
private:
    const int _height;
    const int _width;
    CollisionGrid _position;
};

// ----------------------Character Class----------------------------
class Character : public Entity, public sf::Sprite, public Collidable
{
public:
    Character (const std::string filepath = "Drawing.png");
    Character (const int x, const int y, const std::string filepath);
    ~Character();

    void setxVel(double xVel);
    void setyVel(double yVel);
    void setID(sf::Uint8 ID) {_ID = ID;}
    double getxVel();
    double getyVel();
    sf::Uint8 getID() {return _ID;}
    void setID(int ID) {_ID = ID;}

    void updateChar();
    
    bool collideCheck();
    bool collideX(float moveVal);
    bool collideY(float moveVal);
    bool collideLeft();
    bool collideRight();
    bool collideTop();
    bool collideBottom();

    void transpose(const int &xVal, const int &yVal);
private:
    sf::Texture _texture;
    static vector<Character> charList;

    int numJumps = 0;
    bool secondJump = false;
    double _xVel = 0;
    double _yVel = 0;
    sf::Uint8 _ID= 0;
};

// ----------------------Background Class---------------------------
class Background : public Entity, public sf::Sprite
{
public:
    Background (const std::string filepath);
    ~Background();
private:
    sf::Texture _texture;
};

// -------------------------Block Class------------------------------
class Block : public Entity, public sf::RectangleShape, public Collidable
{
public:
    Block (const int x, const int y, const int width, const int height);
    
    ~Block();
private:
};



#endif /* GAMEBASE_HPP_INCLUDED */
