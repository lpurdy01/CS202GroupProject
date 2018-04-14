// GameBase.hpp
// File containing data for everything within the game environment
//     -Can be seperated to multiple hpp's / cpp's if we need to later


#ifndef GAMEBASE_HPP_INCLUDED
#define GAMEBASE_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>

// ----------------------Entity Class------------------------------
class Entity
{
public:
    Entity (const int x = 0, const int y = 0);
    ~Entity();

    static void addEntity();
    static void deleteEntity();

private:
    static int numEntities;
    int _xCoor;
    int _yCoor;
};

// ----------------------Character Class----------------------------
class Character : public Entity, public sf::Sprite
{
public:
    Character (const std::string filepath);
    Character (const int x, const int y, const std::string filepath);
    ~Character();
private:
    sf::Texture _texture;
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
