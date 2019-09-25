//
// Created by User on 19.07.2019.
//

#ifndef SHAKALAKA_SHAKAL_H
#define SHAKALAKA_SHAKAL_H

#include <SFML/Graphics.hpp>
#include <assert.h>
#include <thread>

#include "Field.h"
#include "units.h"


class player;

class screen
{
public:
    virtual int run(sf::RenderWindow& window) = 0;

};



//setOrigin меняет (0,0) на (x,y) т.е. то что было (0,0) в новых координатах будет (x,y)
class SHAKAL : public screen {
private:

    //std::map<std::string, sf::Texture *>* texture_prefub;
    bool loadedtextures;
    sf::View mainview;

public:
    int TURN;
    const int players_;

    player *PLAYERS[4];

    SHAKAL(int players);

    int run(sf::RenderWindow &window);

    ~SHAKAL();

    int checkWhat(const std::pair<int,int>& pos, TileType type, Pirate* whom);
    int checkScore();

private:


    void proccessEvent(sf::RenderWindow& window);
public:

    struct _collisions
    {
        std::multimap<std::pair<int,int>,Pirate*> friendlyPirates;
        std::multimap<std::pair<int,int>,Pirate*> allPirates;

        _collisions update(Pirate *mov);

    private:
    } collision_;


    void recalculate_cols();
    friend class _CELL;
};

class _CELL : public sf::Drawable {

    friend class SHAKAL;
    SHAKAL& instance;


public:

    _CELL(SHAKAL& ins);

    const std::map<std::pair<int, int>, Tile> &getCoordCell() const;

    std::map<std::pair<int, int>, Tile> &getCoordCell();

    Tile &operator[](const std::pair<int, int> &pos);


    Tile &setDefaultTileSprite(Tile &tile);
private:
    std::map<std::pair<int, int>, Tile> coord_cell;

    float globalX, globalY;
    float scaleX, scaleY, offsetX, offsetY;

public:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;


};



//class WINNER : public screen {
//
//public:
//    sf::Text text;
//    WINNER(int id){
//        text.setFillColor(sf::Color::Blue);
//        text.setFont(fonts->coins_);
//        text.setString("Congratulation! Player " + std::to_string(id) + " won!");
//        text.setCharacterSize(50);
//    };
//
//
//
//    int run(sf::RenderWindow &window) {
//        text.setPosition(window.getView().getCenter());
//
//        while (true) {
//            sf::Event event;
//            while (window.pollEvent(event)) {
//                if (event.type == sf::Event::Closed) {
//                    window.close();
//                    return -1;
//                }
//                window.clear();
//                window.draw(text);
//                window.display();
//            }
//        }
//    };
//
//};



#endif //SHAKALAKA_SHAKAL_H
