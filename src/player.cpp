//
// Created by User on 25.07.2019.
//


#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>
#include <algorithm>
#include <assert.h>
#include <iostream>

#include "units.h"
#include "player.h"
#include <resources.h>
#include <player.h>
#include <GUI.h>

using namespace std;

player::player(int id, std::map<std::string, sf::Texture*>& prefub) : p_id(id),
score(0),rum(0), pirates()
{
    for(int i = 0; i < 3; i++) {
        if (id == 0) {
            if(i == 0) ship = new Ship(make_pair(6,0),*this);
            pirates.push_front(new Pirate(make_pair(6, 0), PIR_STATE, this));
        }
        else if (id == 1) {
            if(i == 0) ship = new Ship(make_pair(0, 6),*this);
            pirates.push_front(new Pirate(make_pair(0, 6), PIR_STATE, this));
        }
        else if (id == 2) {
            if(i == 0) ship = new Ship(make_pair(6, 12),*this);
            pirates.push_front(new Pirate(make_pair(6, 12), PIR_STATE, this));
        }
        else if (id == 3) {
            if(i == 0) ship = new Ship(make_pair(12, 6),*this);
            pirates.push_front(new Pirate(make_pair(12, 6), PIR_STATE, this));
        }
    }

    for(auto &e : pirates)
    {
        e->ENTITYSPRITE.setTexture(*prefub["pirate_red.png"]);
        e->ENTITYSPRITE.setScale(sf::Vector2f(2.3f,2.3f));
        e->ENTITYSPRITE.setOrigin(e->ENTITYSPRITE.getLocalBounds().width/2,e->ENTITYSPRITE.getLocalBounds().height/2);

        ship->add(e);
    }

    ship->ENTITYSPRITE.setTexture(*prefub["ship.png"]);
    ship->ENTITYSPRITE.setScale(0.255f,0.255f);
    ship->ENTITYSPRITE.setOrigin(ship->ENTITYSPRITE.getLocalBounds().width/2,ship->ENTITYSPRITE.getLocalBounds().height/2);

    //ship->ENTITYSPRITE.setOrigin(ship->ENTITYSPRITE.getLocalBounds().width/2,ship->ENTITYSPRITE.getLocalBounds().height/2);
}


void player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    for(auto &e : pirates)
        target.draw(*e, states);
    target.draw(*ship, states);
}

player::~player() {
    for(auto &e : pirates)
        delete(e);
    delete ship;
}

Pirate *player::getPirateById(int id) {
    assert(isMyPir(id));
    return *find_if(pirates.begin(),pirates.end(),[&id](Pirate* p){ return p->getId() == id; });
}

bool player::isMyPir(int pir_id) {
    return find_if(pirates.begin(),pirates.end(),[&pir_id](Pirate* p){ return p->getId() == pir_id; }) != pirates.end();
}

bool player::isMyPir(Pirate *pir) {
    return find_if(pirates.begin(),pirates.end(),[&pir](Pirate* p){ return p == pir; }) != pirates.end();
}

Pirate *player::selectPirate(int position) {
    int i = 0;
    auto e = pirates.begin();

    while(i != position && e != pirates.end())
    {
        e++;
        ++i;
    }

    return *e;
}

void player::remove(Pirate *pir) {
    if(isMyPir(pir))
    {
        pirates.erase(find(pirates.begin(),pirates.end(),pir));
    }
}

void player::update(float time){
    for(auto &e : pirates)
        e->update(time);

    ship->update(time);
}

const list<Pirate *> & player::getPirates() const {
    return pirates;
}

player::player() {
    p_id = -1;
}

int player::getID() const {
    return p_id;
}

void player::kill(Pirate *pir) {
    auto i = find(pirates.begin(),pirates.end(),pir);
    if(i != pirates.end()) {
        ship->add(pir);

        //cout <<  pir->getId() << " pirate was killed\n";
    }
}

Ship *player::getShip()
{
    return ship;
}

void player::cscore() {
    score++;
}

void player::addrum() {
    rum++;
}

void player::userum(Pirate *pirate) {
    rum--;
}

int player::getScore() const {
    return score;
}

int player::getRum() const {
    return rum;
}

void player::born(Pirate *pir) {
    pir->ENTITYSPRITE.setTexture(*textures->textures_.at("pirate_red.png"));
    pir->ENTITYSPRITE.setScale(sf::Vector2f(2.3f,2.3f));
    pir->ENTITYSPRITE.setOrigin(pir->ENTITYSPRITE.getLocalBounds().width/2,
                        pir->ENTITYSPRITE.getLocalBounds().height/2);

    pirates.push_back(pir);
}

std::list<Pirate *> &player::getPirates() {
    return pirates;
}


