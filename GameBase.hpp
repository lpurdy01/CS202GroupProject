// GameBase.hpp
// File containing data for everything within the game environment
//     -Can be seperated to multiple hpp's / cpp's if we need to later


#ifndef GAMEBASE_HPP_INCLUDED
#define GAMEBASE_HPP_INCLUDED
#include <SFML/Graphics.hpp>
#include <string>

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

class Character : public Entity, public sf::Sprite
{
public:
    Character (const int x = 0, const int y = 0, const std::string filepath = "");
    ~Character();
private:
    sf::Texture _texture;
};

#endif /* GAMEBASE_HPP_INCLUDED */
