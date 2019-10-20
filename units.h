//
// Created by User on 18.07.2019.
//

#ifndef SHAKALAKA_UNITS_H
#define SHAKALAKA_UNITS_H

#include <list>
#include <algorithm>
#include "Sbasic.h"
#include "player.h"

class Tile;

class GameItem {
protected:

    GameItem();
    GameItem(const GameItem& item);


    virtual void onDrop() = 0;
    virtual void onPick() = 0;

    virtual void use() = 0;

public:
    enum ItemType{
        NUL = 0,
        GOLD = 1,
        GALEON = 2
    }
            _type;
    ItemType getType() const;

};


class GOLD : public GameItem
{
public:
    GOLD();
    GOLD(const GOLD& gold);

    void use();
    void onDrop();
    void onPick();

};

class GALEON : public GameItem{
public:
    GALEON();
    GALEON(const GALEON& galeon);

    void use();
    void onDrop();
    void onPick();
};


class Pirate : public Entity
{

public:

    Pirate(player *playe);
    Pirate(const Pirate& p);
    Pirate(const std::pair<int, int> &pos, CHECK ch, player *playe);

    const std::list<GameItem *> & getInventory() const;

    void setInventory(const std::list<GameItem *> &inventory);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void update(float time);

    player* Player;

    void pickup(Tile& tile);
    void drop(Tile& tile);

    bool isCarryCoin() const;
    void clearI();

    ~Pirate();

private:
    std::list<GameItem*> INVENTORY;
    int coin;
};


class Ship : public Entity {


public:
    Ship(player& player_);
    Ship(const Ship& sh_);
    Ship(const std::pair<int,int>& pos, player& player);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    void update(float time);

    bool move(const std::pair<int,int>& _p);

    player& Player;

    const std::list<Pirate *> & getPirates() const;

    void add(Pirate* pirate);
    void remove(Pirate* pirate);
    bool isOnShip(Pirate* pirate) const;
private:
    std::list<Pirate*> pirates;

};

#endif //SHAKALAKA_UNITS_H
