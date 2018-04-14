// GameBase.hpp
// File containing data for everything within the game environment
//     -Can be seperated to multiple hpp's / cpp's if we need to later


#ifndef GAMEBASE_HPP_INCLUDED
#define GAMEBASE_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
using std::vector;

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
    int getxPos();
    int getyPos();
private:
    static int numEntities;
    int _xPos;
    int _yPos;
};

// ----------------------Character Class----------------------------
class Character : public Entity, public sf::Sprite
{
public:
    Character (const std::string filepath);
    Character (const int x, const int y, const std::string filepath);
    ~Character();
    
    void setxVel(double xVel);
    void setyVel(double yVel);
    double getxVel();
    double getyVel();
    
    void updateChar();
    
    void transpose(const int xVal, const int yVal);
private:
    sf::Texture _texture;
    static vector<Character> charList;
    
    double _xVel = 0;
    double _yVel = 0;
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
class Block : public Entity, public sf::Shape
{
public:
private:
};



#endif /* GAMEBASE_HPP_INCLUDED */
