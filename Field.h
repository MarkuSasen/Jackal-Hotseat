//
// Created by User on 18.07.2019.
//

#ifndef SHAKALAKA_FIELD_H
#define SHAKALAKA_FIELD_H

#include <map>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <iostream>

#include "Sbasic.h"
#include "units.h"

enum TileType {
    VOID,
    CLEAR,
    SEA,
    ARROW1,
    ARROW2,
    ARROW3,
    ARROW4,
    ARROW5,
    ARROW6,
    ARROW7,
    HORSES,
    TURNTABLE2,
    TURNTABLE3,
    TURNTABLE4,
    TURNTABLE5,
    ICE,
    TRAP,
    CANNIBAL,
    FORTRESS,
    COINS1,
    COINS2,
    COINS3,
    COINS4,
    COINS5,
    TREASURE,
    PLANE,
    BALOON,
    CANNON,
    LIGHTHOUSE,
    BEN,
    MISSIONARY,
    FRIDAY,
    RUM1,
    RUM2,
    RUM3,
    RUM_BARREL,
    CAVE,
    EARTHQUAKE,
    JUNGLE,
    WEED,
    BOAT,
    KERNELS,
    CART,
    CROCODILE,
    ABORIGINAL
};

typedef void (*ACTION)(Pirate *p);

class _CELL;

struct tile_action {
    tile_action(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    virtual void operator=(tile_action &tile);

    Tile *t;
    std::list<Pirate *> _pirates;
    _CELL *_cell;
    bool used;

    std::pair<int, int> last_position;
};

struct REGULAR_ACTION : public tile_action {
    REGULAR_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

};

struct HORSE_ACTION : public tile_action {
    HORSE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

};

struct ICE_ACTION : public tile_action {
    ICE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};

struct ARROWS : public tile_action {
    ARROWS(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    int rotate;
    bool rotated;

};


struct RUMBARREL_ACTION : public tile_action {
    RUMBARREL_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    std::list<Pirate *> zapoy;
};

struct TRAP_ACTION : public tile_action {
    TRAP_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    Pirate *_trapped;
   // Pirate *_halped;
};


struct VERTEX : public tile_action {
    VERTEX(Tile *t, int turns, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    void use_rum(Pirate *pir);


    enum _position : int {
        I = 1, II = 2, III = 3, IV = 4, V = 5
    };
    int _turns;

    std::multimap<_position, Pirate *> _trapped;
};


struct CROCO_ACTION : public tile_action {
    CROCO_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};

struct CANNIBAL_ACTION : public tile_action {
    CANNIBAL_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

};

struct CASTLE_ACTION : public tile_action {
    CASTLE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

};

struct ABORIGINAL_ACTION : public tile_action {
    ABORIGINAL_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    bool voskreshaem;
    player* _pl;
};


struct BALLOON_ACTION : public tile_action {
    BALLOON_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};


struct PLANE_ACTION : public tile_action {
    PLANE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    bool triggered, used;
    int capacity;
};

struct RUM_ACTION : public tile_action {
    RUM_ACTION(Tile *t, _CELL *cell, int amount);

    virtual void update();

    virtual int operator()(Pirate *pir);

    int _amount;
};

struct EARTHQUAKE_ACTION : public tile_action {
    EARTHQUAKE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};


struct GALEON_ACTION : public tile_action {
    GALEON_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

};


struct CAVE_ACTION : public tile_action {
    CAVE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    Tile *exit1, *exit2;
    std::pair<int,int> e1,e2;
};

class SHAKAL;

struct WEED_ACTION : public tile_action {
    WEED_ACTION(Tile *t, _CELL *cell, SHAKAL *field);

    virtual void update();

    virtual int operator()(Pirate *pir);

    SHAKAL *_field;
    player *_stepped;

    int _turn;
};


struct GOLD_ACTION : public tile_action {
    GOLD_ACTION(Tile *t, _CELL *cell, int num);

    virtual void update();

    virtual int operator()(Pirate *pir);

    int _num;
};


struct BEACON_ACTION : public tile_action {
    BEACON_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};


struct JUNGLE_ACTION : public tile_action {
    JUNGLE_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};


struct SEA_ACTION : public tile_action {
    SEA_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};


struct CANON_ACTION : public tile_action {
    CANON_ACTION(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);

    int rotate;
    bool rotated;
};

struct ADDITIONAL_PIRATES : public tile_action {
    ADDITIONAL_PIRATES(Tile *t, _CELL *cell);

    virtual void update();

    virtual int operator()(Pirate *pir);
};



class Tile : public sf::Drawable {

public:
    Tile(TileType t = VOID, tile_action *act = nullptr);

    Tile(const Tile &tile);

    TileType getTiletype() const;

    void setTiletype(TileType tiletype);

    bool isHidden() const;

    void show();

    bool operator==(const Tile &tile);
    Tile& operator=(const Tile& tile);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    const sf::Sprite &getSprite() const;

    void setSprite(const sf::Sprite &sprite);

    void _do(Pirate *pir);

    bool hasPirate(Pirate *pir);

    void removePir(Pirate *pir);

    GameItem *removeItem();

    void addItem(GameItem *item);

    bool hasCoins() const;

    bool hasTreasure() const;

    const std::list<GameItem *> &getItems() const;

    tile_action *A;

private:
    TileType tiletype;
    bool hidden;
    int _coins;
public:
    const std::list<Pirate *> &getPirates() const;

private:
    bool allow_coins, allow_fight, allow_drop;
    bool multiple_pirates;
public:
    bool isAllowCoins() const;

    void setAllowCoins(bool allowCoins);

    bool isAllowFight() const;

    void setAllowFight(bool allowFight);

    bool isAllowDrop() const;

    void setAllowDrop(bool allowDrop);

    bool isMultiplePirates() const;

    void setMultiplePirates(bool multiplePirates);


private:

    std::list<GameItem *> _items;

public:
    sf::Sprite sprite, rubashka;
    sf::Text coin_number, galeon_existing;
};


#endif //SHAKALAKA_FIELD_H
