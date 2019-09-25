//
// Created by User on 19.07.2019.
//
#include "Field.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <iostream>
#include <resources.h>
#include <numeric>
#include "SHAKAL.h"

using namespace std;
using namespace sf;


std::array<TileType,8> killable = { TileType::ICE,
                        TileType::ARROW1,
                        TileType::ARROW2,
                        TileType::ARROW3,
                        TileType::ARROW4,
                        TileType::ARROW5,
                        TileType::ARROW6,
                        TileType::ARROW7 };


Tile::Tile(TileType t, tile_action *act) : tiletype(t), hidden(true), _coins(0),
                                           allow_coins(true), allow_fight(true),
                                           multiple_pirates(false), allow_drop(true), A(act)
{
    coin_number.setFont(fonts->coins_);
    coin_number.setCharacterSize(30);
    coin_number.setFillColor(Color::Cyan);

    galeon_existing.setFont(fonts->coins_);
    galeon_existing.setCharacterSize(30);
    galeon_existing.setFillColor(Color(182,25,25,255));


    galeon_existing.setPosition(sprite.getGlobalBounds().left,
            sprite.getGlobalBounds().top);
    coin_number.setPosition(sprite.getGlobalBounds().left,
            sprite.getGlobalBounds().top);
}


Tile::Tile(const Tile& tile) : tiletype(tile.getTiletype()), allow_fight(tile.allow_fight),
                               allow_coins(tile.allow_coins), multiple_pirates(tile.multiple_pirates),hidden(tile.isHidden()),
                               _items(tile._items), allow_drop(tile.allow_drop), A(tile.A),sprite(tile.sprite),
                               coin_number(tile.coin_number),galeon_existing(tile.galeon_existing){}

TileType Tile::getTiletype() const {
    return tiletype;
}

void Tile::setTiletype(TileType tiletype) {
    this->tiletype = tiletype;
}

bool Tile::isHidden() const {
    return hidden;
}

void Tile::show() {
    hidden = false;
}

bool Tile::operator==(const Tile& tile){
    return tile.getTiletype() == tiletype;
}

void Tile::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    target.draw(sprite, states);
    if (hasTreasure()) {
        target.draw(galeon_existing, states);
    }
    if(hasCoins())
        target.draw(coin_number, states);
}

const sf::Sprite &Tile::getSprite() const {
    return sprite;
}

void Tile::setSprite(const sf::Sprite &sprite) {
    Tile::sprite = sprite;
}

Tile &Tile::operator=(const Tile &tile) {
    tiletype = tile.getTiletype();
    allow_fight = tile.allow_fight;
    allow_coins = tile.allow_coins;
    multiple_pirates = tile.multiple_pirates;
    hidden = tile.hidden;
    _items = tile._items;
    //if(A && tile.A) {
    //std::cout << A << " -> " ;
    if(A && tile.A){
        A->_cell->setDefaultTileSprite(*this);
        //A->t = const_cast<Tile*>(&tile);
    }
    //std::cout << A << std::endl;


    if(tile.sprite.getTexture() != nullptr)
        sprite.setTexture(const_cast<const Texture&>(*tile.sprite.getTexture()));
    //sprite.setPosition(tile.sprite.getPosition());


    return *this;
}


void Tile::_do(Pirate *pir) {
    show();
    (*A)(pir);
}

bool Tile::hasPirate(Pirate *pir) {
    return std::find_if(A->_pirates.begin(), A->_pirates.end(), [&pir](Pirate* p){
        return p->getId() == pir->getId();
    }) != A->_pirates.end();
}

bool Tile::hasCoins() const {
    return std::find_if(_items.begin(), _items.end(), [](GameItem* p){
        return p->getType() == 1;
    }) != _items.end();
}

bool Tile::hasTreasure() const {
    return std::find_if(_items.begin(), _items.end(), [](GameItem* p){
        return p->getType() == GameItem::ItemType::GALEON;
    }) != _items.end();
}

GameItem * Tile::removeItem() {

    GameItem* hehe = nullptr;

    if(_items.begin() != _items.end())
    {
        hehe = *_items.begin();
        _items.erase(_items.begin());
    }

    coin_number.setString(std::to_string(std::count_if( _items.begin(),_items.end(),
            [](GameItem* item){ return item->getType() == GameItem::ItemType::GOLD;})));
    galeon_existing.setString(std::to_string(std::count_if( _items.begin(),_items.end(),
            [](GameItem* item){ return item->getType() == GameItem::ItemType::GALEON;})));

    return hehe;
}

void Tile::addItem(GameItem *item) {

    if(!item) return;

    _items.push_back(item);
    galeon_existing.setString(std::to_string(std::count_if( _items.begin(),_items.end(),
                                                            [](GameItem* item){ return item->getType() == GameItem::ItemType::GALEON;})));
    coin_number.setString(std::to_string(std::count_if( _items.begin(),_items.end(),
                                                        [](GameItem* item){ return item->getType() == GameItem::ItemType::GOLD;})));
}

void Tile::removePir(Pirate *pir) {
    if(hasPirate(pir))
        A->_pirates.erase(find(A->_pirates.begin(),A->_pirates.end(),pir));
}

const std::list<Pirate *> &Tile::getPirates() const {
    return A->_pirates;
}

bool Tile::isAllowCoins() const {
    return allow_coins;
}

void Tile::setAllowCoins(bool allowCoins) {
    allow_coins = allowCoins;
}

bool Tile::isAllowFight() const {
    return allow_fight;
}

void Tile::setAllowFight(bool allowFight) {
    allow_fight = allowFight;
}

bool Tile::isAllowDrop() const {
    return allow_drop;
}

void Tile::setAllowDrop(bool allowDrop) {
    allow_drop = allowDrop;
}

bool Tile::isMultiplePirates() const {
    return multiple_pirates;
}




void Tile::setMultiplePirates(bool multiplePirates) {
    multiple_pirates = multiplePirates;
}

const std::list<GameItem *> &Tile::getItems() const {
    return _items;
}
////////////////////////////////////////
///////////////////////////////////////


//////////////////////////////////////

tile_action::tile_action(Tile *t,_CELL* cell) : t(t),_cell(cell), used(false){
    t->setAllowCoins(true);
    t->setAllowDrop(true);
    t->setAllowFight(true);
    t->setMultiplePirates(false);
}

void tile_action::operator()(Pirate *pir) {
    if(!pir)
        return;

    auto e_count = std::count_if(_pirates.begin(),_pirates.end(),[&pir](Pirate* p)
                           {
                               return p->Player->getID() != pir->Player->getID();
                           }
    );

    if(!t->isMultiplePirates())
    {
        if(e_count) {

            auto true_prev = pir->getPreviousPos();// _cell->getCoordCell()[pir->getPreviousPos()].A->last_position;

            if(std::find(killable.begin(),killable.end(),_cell->getCoordCell()[true_prev].getTiletype()) != killable.end())
            {
//                Tile *prevtile = &_cell->getCoordCell()[pir->getPreviousPos()];
//                if(prevtile->hasPirate(pir))
//                    prevtile->A->_pirates.erase(find(prevtile->A->_pirates.begin(),prevtile->A->_pirates.end(),pir));

                pir->Player->kill(pir);
                return;
            }

            pir->move(pir->getPreviousPos(),FORCEMOVE);
            pir->setPreviousPos(true_prev);
            pir->setCanmove(true);

            return;
        }
    }

    if(t->isAllowFight())
    {
        if(e_count)
        {
            for(auto &e : _pirates) {

                if(e->Player->getID() == pir->Player->getID()) continue;

                e->drop(*t);

                Tile *prevtile = &_cell->getCoordCell()[e->getPreviousPos()];
                if(prevtile->hasPirate(e))
                    prevtile->A->_pirates.erase(find(prevtile->A->_pirates.begin(),prevtile->A->_pirates.end(),e));

                e->Player->kill(e);

            }
        }
    }

    if(!t->isAllowCoins())
    {
        if(pir->isCarryCoin())
        {
            auto last = pir->getPreviousPos();
            bool wheretodrop = _cell->getCoordCell()[last].isAllowDrop();
            while(!wheretodrop)
            {
                last = _cell->getCoordCell()[last].A->last_position;
                wheretodrop = _cell->getCoordCell()[last].isAllowDrop();
            }

            pir->drop(_cell->getCoordCell()[last]);
        }
    }

    if(!t->hasPirate(pir)) {
        _pirates.push_back(pir);
        last_position = pir->getPreviousPos();
    }

    Tile *prevtile = &_cell->getCoordCell()[pir->getPreviousPos()];
        if(prevtile->hasPirate(pir))
            prevtile->A->_pirates.erase(find(prevtile->A->_pirates.begin(),prevtile->A->_pirates.end(),pir));

    for (auto &e : pir->Player->getPirates())
        e->setCanmove(false);

        pir->Player->getShip()->setCanmove(false);

        pir->setSTATE(PIR_STATE);

}

void tile_action::update()
{}

void tile_action::operator=(tile_action &tile) {
    _cell = tile._cell;
    _pirates = tile._pirates;
    last_position = tile.last_position;
    std::cout << t << " -> ";
    this->t = tile.t;
    std::cout << t << std::endl;
    used = tile.used;
}

////////////////////////////////////

REGULAR_ACTION::REGULAR_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void REGULAR_ACTION::operator()(Pirate *pir)
{
    tile_action::operator()(pir);
    //std::cout << "REGULAR_ACTION\n";
}

void REGULAR_ACTION::update()
{}

/////////////////////////////////

HORSE_ACTION::HORSE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(false);
}

void HORSE_ACTION::operator()(Pirate *pir)
{
    tile_action::operator()(pir);
    pir->setCanmove(true);
    pir->setSTATE(HORSE_STATE);
}

void HORSE_ACTION::update()
{

}

//////////////////////////////

ICE_ACTION::ICE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(false);
}

void ICE_ACTION::operator()(Pirate *pir) {

    if(pir->getSTATE() == HORSE_STATE)
    {
        tile_action::operator()(pir);
        pir->setSTATE(HORSE_STATE);
        pir->setCanmove(true);
    }else if(pir->getSTATE() == FORCEMOVE)
    {
        tile_action::operator()(pir);
        pir->setSTATE(FORCEMOVE);
        pir->setCanmove(true);
    }
    else {
        last_position = pir->getPreviousPos();
        pir->move(std::make_pair(
                -1 * (last_position.first - pir->getPos().first) + pir->getPos().first,
                -1 * (last_position.second - pir->getPos().second) + pir->getPos().second), FORCEMOVE);
        _cell->getCoordCell()[pir->getPos()]._do(pir);
        //tile_action::operator()(pir);
    }
}

void ICE_ACTION::update() {

}

//////////////////////////////

ARROWS::ARROWS(Tile *t, _CELL *cell) : tile_action(t, cell), rotated(0), rotate(0)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(false);
}

void ARROWS::operator()(Pirate *pir) {
    if(!rotated) {
        std::srand(std::chrono::system_clock::now().time_since_epoch().count());
        rotate = std::rand() % 4;
        rotated = true;
        t->sprite.rotate(rotate * 90);
    }
    int x = pir->getX();
    int y = pir->getY();
    std::pair<int,int> vec;

    tile_action::operator()(pir);

    if(t->getTiletype() == ARROW1){

        vec = rotate == 3 ? make_pair(x + -1, y + 0) :
              rotate == 2 ? make_pair(x + 0, y + 1) :
              rotate == 1 ? make_pair(x + 1, y + 0) :
              make_pair(x + 0, y + -1);

        pir->move(vec,FORCEMOVE);
        _cell->getCoordCell()[pir->getPos()]._do(pir);
    } else if(t->getTiletype() == ARROW2)
    {
        vec = rotate == 3 ? make_pair(x + -1, y + -1) :
              rotate == 2 ? make_pair(x + -1, y + 1) :
              rotate == 1 ? make_pair(x + 1, y + 1) :
              make_pair(x + 1, y + -1);

        pir->move(vec,FORCEMOVE);
        _cell->getCoordCell()[pir->getPos()]._do(pir);
    } else if(t->getTiletype() == ARROW3)
    {
        pir->setCanmove(true);
        rotate % 2 == 0 ? pir->setSTATE(ARROW30_STATE)
        : pir->setSTATE(ARROW31_STATE);
    }
    else if(t->getTiletype() == ARROW4)
    {
        pir->setCanmove(true);
        rotate % 2 == 0 ? pir->setSTATE(ARROW40_STATE)
                        : pir->setSTATE(ARROW41_STATE);
    }else if(t->getTiletype() == ARROW5)
    {
        pir->setCanmove(true);
        rotate == 3 ? pir->setSTATE(ARROW51_STATE) :
        rotate == 2 ? pir->setSTATE(ARROW50_STATE) :
        rotate == 1 ? pir->setSTATE(ARROW53_STATE) :
            pir->setSTATE(ARROW52_STATE);


    }else if(t->getTiletype() == ARROW6){
        pir->setCanmove(true);
        pir->setSTATE(ARROW6_STATE);

    }else if(t->getTiletype() == ARROW7){
        pir->setCanmove(true);
        pir->setSTATE(ARROW7_STATE);
    }


}

void ARROWS::update() {}

//////////////////////////////

RUMBARREL_ACTION::RUMBARREL_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void RUMBARREL_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);
    //zapoy.push_back(pir);
}


void RUMBARREL_ACTION::update() {
    if(!_pirates.empty())
        for(auto &e : _pirates)
        {
            if(e->isCanmove())
            {
                e->setCanmove(false);
                _pirates.erase(std::find(_pirates.begin(),_pirates.end(),e));
            }
        }
}


//////////////////////////////

TRAP_ACTION::TRAP_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell), _trapped(nullptr)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void TRAP_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);
    if(_trapped)
        if(pir->Player->getID() == _trapped->Player->getID())
        {
            _trapped = nullptr;
        } else {
            _trapped->Player->kill(_trapped);
            _trapped = pir;
        }
    else _trapped = pir;
}


void TRAP_ACTION::update() {
    if(_trapped) _trapped->setCanmove(false);
}


//////////////////////////////

VERTEX::VERTEX(Tile *t, int turns, _CELL *cell) : tile_action(t, cell), _turns(turns)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(true);
    t->setAllowFight(false);
    t->setAllowDrop(true);
}

void VERTEX::operator()(Pirate *pir) {
    pir->setPreviousPos(pir->getPos());
    tile_action::operator()(pir);

    multimap<_position , Pirate*>::iterator pir_iter = _trapped.end();

    for(int i = 1; i <= _turns; i++){
        auto e = _trapped.lower_bound(static_cast<_position >(i));
        for(auto c = e; c != _trapped.upper_bound(static_cast<_position >(i)); c++) {
            cout << (*c).first << " " << (*c).second->getId() << "\n";
            if ((*c).second == pir) {
                pir_iter = c;
                break;
            }
        }
    }

    if(pir_iter == _trapped.end())
    {
        _trapped.insert((make_pair(I,pir)));
    }else {
        _position temp = (*pir_iter).first;
        _trapped.erase(pir_iter);

        if(temp+1 <= _turns)
            pir_iter = _trapped.insert(make_pair(static_cast<_position>(temp+1), pir));
        else {
            pir->setSTATE(PIR_STATE);
            return;
        }
    }

    auto range = _trapped.equal_range((*pir_iter).first);

    for(auto i = range.first; i != range.second; i++){
        cout << (*i).second << " \n";
        if((*i).second->Player->getID() != pir->Player->getID())
        {
            (*i).second->Player->kill((*i).second);
            _trapped.erase(i);
        }
    }

    pir->setSTATE(VERTEX_STATE);

}

void VERTEX::update() {

}

void VERTEX::use_rum(Pirate *pir) {
    /*auto e = find(_trapped.begin(),_trapped.end(),pir);
    if(e != _trapped.end()) {
        (*e).second->Player.userum(pir);

        _position temp = (*e).first;
        _trapped.erase(e);

        if(temp+1 <= _turns)
            _trapped.insert(make_pair(static_cast<_position>(temp+1), pir));
    }*/
}

///////////////////////////////

CROCO_ACTION::CROCO_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(false);
    t->setAllowDrop(false);
}

void CROCO_ACTION::operator()(Pirate *pir) {

    tile_action::operator()(pir);

    if(_cell->getCoordCell()[last_position].getTiletype() == ARROW1
    || _cell->getCoordCell()[last_position].getTiletype() == ARROW2
    || _cell->getCoordCell()[last_position].getTiletype() == ICE)
    {
        _pirates.erase(find(_pirates.begin(),_pirates.end(), pir));
        pir->Player->remove(pir);
        return;
    }
    std::pair<int,int> prev = pir->getPreviousPos();
    pir->move(prev,FORCEMOVE);
    _cell->getCoordCell()[prev]._do(pir);
}

void CROCO_ACTION::update() {}

///////////////////////////////

CANNIBAL_ACTION::CANNIBAL_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(false);
}

void CANNIBAL_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    _pirates.erase(find(_pirates.begin(),_pirates.end(),pir));
    pir->Player->remove(pir);
}

void CANNIBAL_ACTION::update() {}


///////////////////////////////

CASTLE_ACTION::CASTLE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(false);
    t->setMultiplePirates(false);
    t->setAllowFight(false);
    t->setAllowDrop(false);
}

void CASTLE_ACTION::operator()(Pirate *pir) {

    tile_action::operator()(pir);

    if(count_if(_pirates.begin(),_pirates.end(),[&pir](Pirate * _p)
    {
       return _p->Player->getID() != pir->Player->getID();
    }))
    {
        pir->Player->remove(pir);
        return;
    }
}

void CASTLE_ACTION::update() {

}


//////////////////////////////

ABORIGINAL_ACTION::ABORIGINAL_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell),voskreshaem(false)
{
    t->setAllowCoins(false);
    t->setMultiplePirates(false);
    t->setAllowFight(false);
    t->setAllowDrop(false);
}

void ABORIGINAL_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    if(count_if(_pirates.begin(),_pirates.end(),[&pir](Pirate * _p)
    {
        return _p->Player->getID() != pir->Player->getID();
    }))
    {
        pir->Player->remove(pir);
        return;
    }

    if(pir->Player->getPirates().size() < 3)
    {
        voskreshaem = true;
    }
}

void ABORIGINAL_ACTION::update() {
    if(voskreshaem && !_pirates.empty())
    {
        (*_pirates.begin())->setCanmove(false);
        voskreshaem = false;
        std::pair<int,int> pos = (*_pirates.begin())->getPos();
        (*_pirates.begin())->Player->born(new Pirate(pos, PIR_STATE,(*_pirates.begin())->Player));
        _pirates.push_back((*_pirates.begin())->Player->getPirates().back());
    }
}


/////////////////////////////////

BALLOON_ACTION::BALLOON_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(false);
}

void BALLOON_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    _pirates.erase(find(_pirates.begin(),_pirates.end(),pir));
    pir->Player->getShip()->add(pir);
}

void BALLOON_ACTION::update() {

}


/////////////////////////////////


PLANE_ACTION::PLANE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell),triggered(false),capacity(0)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void PLANE_ACTION::operator()(Pirate *pir) {

    tile_action::operator()(pir);

    if(used)
        return;

    triggered = true;
    pir->setSTATE(PLANE_STATE);

}


void PLANE_ACTION::update() {


    if(triggered)
    {
        if(capacity > t->getPirates().size())
            used = true;
    }

    if(used)
    {
        for(auto &e : _pirates)
        {
            e->setSTATE(PIR_STATE);
        }
        return;
    }

    capacity = _pirates.size();
}

/////////////////////////////////

RUM_ACTION::RUM_ACTION(Tile *t, _CELL *cell, int amount) : tile_action(t, cell), _amount(amount)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void RUM_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);
    if(used) return;

    for ( int i = 0; i < _amount; i++)
        pir->Player->addrum();

    used = true;
}

void RUM_ACTION::update() {

}

/////////////////////////////////

EARTHQUAKE_ACTION::EARTHQUAKE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void EARTHQUAKE_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    if(used)
        return;

    pir->setSTATE(ONSHIP);
    pir->setCanmove(true);

    used = true;
}

void EARTHQUAKE_ACTION::update() {

}


///////////////////////////

GALEON_ACTION::GALEON_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void GALEON_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);
    if(used)
        return;

    (*_cell)[pir->getPos()].addItem(new GALEON());
    used = true;
//
//    t.galeon_existing.setString(std::to_string(std::count_if( t.getItems().begin(),t.getItems().end(),
//                                                            [](GameItem* item){ return item->getType() == GameItem::ItemType::GALEON;})));
}

void GALEON_ACTION::update() {

}

///////////////////////////

CAVE_ACTION::CAVE_ACTION(Tile *t, _CELL *cell) : tile_action(t,cell), exit1(nullptr), exit2(nullptr){

        t->setAllowCoins(true);
        t->setMultiplePirates(false);
        t->setAllowFight(false);
        t->setAllowDrop(true);

}

void CAVE_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    if(!used)
        for(int i = 0; i < 13; i++) {
            for (int j = 0; j < 13; j++) {

                if (_cell->getCoordCell()[make_pair(i,j)].getTiletype() == TileType::CAVE &&
                    &_cell->getCoordCell()[make_pair(i,j)] != t) {
                    if (!exit1) {
                        exit1 = &_cell->getCoordCell()[make_pair(i,j)];
                        e1 = make_pair(i,j);
                        cout << t << " e1 -> " << exit1 << endl;
                    } else {
                        exit2 = &_cell->getCoordCell()[make_pair(i,j)];
                        e2 = make_pair(i,j);
                        cout << t << " e2 -> " << exit2 << endl;
                    }
                }

            }
        }
    used = true;

    if(!exit1->isHidden() && !exit2->isHidden())
    {
        if(!exit1->getPirates().empty()
                && exit1->getPirates().front()->Player->getID() != pir->Player->getID()
        && !exit2->getPirates().empty()
            && exit2->getPirates().front()->Player->getID() != pir->Player->getID())
                return;

        pir->setSTATE(ONSHIP);
        pir->setCanmove(true);
        return;

    }else if(!exit1->isHidden() && exit2->isHidden())
    {
        if(!exit1->getPirates().empty())
        {
            if(exit1->getPirates().front()->Player->getID() ==
                                                    pir->Player->getID())
            {
                pir->move(e1,FORCEMOVE);
            } else return;
        } else pir->move(e1,FORCEMOVE);

    }else if(!exit2->isHidden() && exit1->isHidden())
    {
        if(!exit2->getPirates().empty())
        {
            if(exit2->getPirates().front()->Player->getID() ==
               pir->Player->getID())
            {
                pir->move(e2,FORCEMOVE);
            } else return;
        } else pir->move(e2,FORCEMOVE);
    }
}

void CAVE_ACTION::update() {
    if(used)
        if(exit1->isHidden() && exit2->isHidden())
            for(auto &e : _pirates)
                e->setCanmove(false);
}

///////////////////////////

WEED_ACTION::WEED_ACTION(Tile *t, _CELL *cell, SHAKAL *field): tile_action(t, cell),
                                                               _field(field), _turn(0)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void WEED_ACTION::operator()(Pirate *pir) {
#define P(C) _field->PLAYERS[C]

    tile_action::operator()(pir);
    if(used) return;

    used = true;

    _stepped = pir->Player;
    _turn = _field->TURN;
//
//    for(int i = 1; i < _field->players_; i++)
//    {
//        swap(P(0)->ship,P(i)->ship);
//        P(0)->getPirates().swap(P(i)->getPirates());
//        for(auto &e : P(i)->getPirates())
//         {
//             swap(e->Player , P(0)->selectPirate(0)->Player);
//             cout << "\n| " << e->getId() << " | " << e->Player << " |";
//         }
//    }

    for(int i = _field->players_-1; i > 0; i--)
        swap(P(0), P(i));

}

void WEED_ACTION::update() {

    if(used)
    {
        if(_turn + _field->players_+1 == _field->TURN){

            for(int i = _field->players_-1; i > 0; i--)
                swap(P(0), P(i));

//            for(int i = _field->players_-1; i > 0; i--)
//            {
//                swap(P(0)->ship,P(i)->ship);
//                P(0)->getPirates().swap(P(i)->getPirates());
//                for(auto &e : P(i)->getPirates())
//                {
//                    swap(e->Player , P(0)->selectPirate(0)->Player);
//                    cout << "\n| " << e->getId() << " | " << e->Player << " |";
//                }
//
//            }
        }
    }
}
#undef P
///////////////////////////


GOLD_ACTION::GOLD_ACTION(Tile *t, _CELL *cell, int num) : tile_action(t, cell), _num(num)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void GOLD_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    if(used) return;

    used = true;
    cout << "spawned " << _num << endl;
    for(int i = 0; i < _num; ++i)
        t->addItem(new GOLD());
}

void GOLD_ACTION::update() {

}

///////////////////////////

BEACON_ACTION::BEACON_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(false);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}

void BEACON_ACTION::operator()(Pirate *pir) {
    tile_action::operator()(pir);

    if(used)
        return;
    used = true;

    pir->setSTATE(ONSHIP);
    pir->setCanmove(true);
}

void BEACON_ACTION::update() {

}

/////////////////////////

JUNGLE_ACTION::JUNGLE_ACTION(Tile *t, _CELL *cell) : tile_action(t, cell)
{
    t->setAllowCoins(true);
    t->setMultiplePirates(true);
    t->setAllowFight(true);
    t->setAllowDrop(true);
}


void JUNGLE_ACTION::operator()(Pirate *pir) {

}


void JUNGLE_ACTION::update() {

}