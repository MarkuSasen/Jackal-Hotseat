//
// Created by User on 19.07.2019.
//
#include "units.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <iostream>
#include <Field.h>
#include <complex.h>
#include <algorithm>


Pirate::Pirate(player *playe) : Entity(), INVENTORY(), Player(playe), coin(0) {
    INVENTORY.clear();
}
Pirate::Pirate(const Pirate& p) : Entity(p), INVENTORY(p.getInventory()), Player(p.Player), coin(0) {}
Pirate::Pirate(const std::pair<int, int> &pos, CHECK ch, player *playe) : Entity(pos, ch), Player(playe), coin(0) {}

const std::list<GameItem *> & Pirate::getInventory() const
{
    return INVENTORY;
}

void Pirate::setInventory(const std::list<GameItem *> &inventory)
{
    INVENTORY = inventory;
}

void Pirate::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //отступ от угла
    states.transform *= sf::Transform().translate(52,52);
    if(astate == MOVE)
    {
        states.transform *= sf::Transform().translate(getPreviousPos().first*104 - 52,
                getPreviousPos().second * 104 - 52);

    }else states.transform *= sf::Transform().translate(getX()*104 - 52, getY()* 104 - 52);

    target.draw(ENTITYSPRITE,states);
}

void Pirate::update(float time) {
    Entity::update(time);

    if(astate == SELECTED) {
        if (localtime < 0.5 && localtime >= 0) {
            ENTITYSPRITE.scale(1 + time * 0.0005, 1 + time * 0.0005);
            localtime += time * 0.0005;

        } else if (localtime >= 0.5f && localtime < 1.0f) {
            ENTITYSPRITE.scale(1 - time * 0.0005, 1 - time * 0.0005);
            localtime += time * 0.0005;

        } else localtime = 0;
    }else if(astate == MOVE)
    {
        sf::Vector2f vec(getPos().first - getPreviousPos().first,
                         getPos().second - getPreviousPos().second);
        localtime += 1+time*0.005f;
        ENTITYSPRITE.move(((1+time*0.005f)*vec.x),((1+time*0.005f)*vec.y));


        if(localtime >= 104)
        {
            astate = Entity::AnimType::NONE;
            ENTITYSPRITE.move(-localtime*vec.x,-localtime*vec.y);
            localtime = 0;
            resetSprite(2.3f,2.3f,0,0);
        }
    } else if(astate == NONE)
    {
        localtime = 0;
    }

}

void Pirate::pickup(Tile &tile) {
        if(!isCarryCoin())
        {
            GameItem* hoho = tile.removeItem();
            if(hoho)
                INVENTORY.push_back(hoho);
        }
}

void Pirate::drop(Tile& tile) {
    if(isCarryCoin()) {
        if(INVENTORY.begin() != INVENTORY.end())
        {
            tile.addItem(*INVENTORY.begin());
            INVENTORY.erase(INVENTORY.begin());
        }
    }

}


//несет ли сокровище или монету
bool Pirate::isCarryCoin() const {
    return std::find_if(INVENTORY.begin(),INVENTORY.end(),
            [](GameItem* item){ return item->getType() == 1 || item->getType() == 2; }) != INVENTORY.end();
}

void Pirate::clearI() {
    INVENTORY.clear();
}

/////////////////////////////
/////////////////////////////
/////////////////////////////

GameItem::GameItem() : _type(NUL) {

}

GameItem::GameItem(const GameItem &item) : _type(item.getType()) {

}

GameItem::ItemType GameItem::getType() const {
    return _type;
}

/////////////////////////////
/////////////////////////////
/////////////////////////////

GOLD::GOLD() : GameItem() {
    _type = ItemType::GOLD;
}

GOLD::GOLD(const GOLD &gold) : GameItem(gold) {

}

void GOLD::use() {

}

void GOLD::onDrop() {

}

void GOLD::onPick() {

}

GALEON::GALEON() {
    _type = ItemType::GALEON;
}

GALEON::GALEON(const GALEON &galeon) : GameItem(galeon) {

}

void GALEON::use() {

}

void GALEON::onDrop() {

}

void GALEON::onPick() {

}

/////////////////////////////
/////////////////////////////
/////////////////////////////////


Ship::Ship(player &player_) : Entity(), Player(player_){
    setSTATE(FREGATH_STATE);
}


Ship::Ship(const Ship &sh_) : Entity(sh_), Player(sh_.Player), pirates(sh_.getPirates())
{

}

Ship::Ship(const std::pair<int, int> &pos, player &player) : Entity(pos, FREGATH_STATE),
Player(player){

}

bool Ship::move(const std::pair<int, int> &_p) {

    if(pirates.size() < 1) return false;

    bool moved = Entity::move(_p);

    if(moved)
    {
        for(auto &e : pirates) {
            e->setPreviousPos(e->getPos());
            e->setPos(_p.first, _p.second);
        }
    }
    return moved;
}


void Ship::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= sf::Transform().translate(getX()*104.3f, getY() * 104.3f);
    target.draw(ENTITYSPRITE,states);
}

void Ship::update(float time)
{
    if(astate == AnimType::SELECTED)
    {
        ENTITYSPRITE.setColor(sf::Color::Red);
    }
}

const std::list<Pirate *> &Ship::getPirates() const {
    return pirates;
}



void Ship::add(Pirate *pirate) {

   if(!isOnShip(pirate))
   {
       pirates.push_back(pirate);
       pirate->setSTATE(ONSHIP);
       pirate->setPreviousPos(pirate->getPos());
       pirate->setPos(getPos());


       for(auto &e : pirate->getInventory())
       {
           if(e->getType() == GameItem::ItemType::GALEON)
               for(int i = 0; i < 3; i++) Player.cscore();
           else
               Player.cscore();
       }

       pirate->clearI();
   }
}

void Ship::remove(Pirate *pirate) {
    if(isOnShip(pirate)) {
        pirates.erase(std::find(pirates.begin(), pirates.end(), pirate));

        if(pirates.empty()) setCanmove(false);
    }
}

bool Ship::isOnShip(Pirate *pirate) const {
    return std::find(pirates.begin(),pirates.end(),pirate) != pirates.end();
}

///////////////////////////////////////////



