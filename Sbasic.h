//
// Created by User on 18.07.2019.
//

#ifndef SHAKALAKA_SBASIC_H
#define SHAKALAKA_SBASIC_H

#include <utility>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>

typedef bool (*CHECK)(const std::pair<int,int>& prev, const std::pair<int,int>& curr);
static unsigned int E_count = 0;


class Entity : public sf::Drawable{

protected:

    Entity();

    Entity(const Entity& e);

    Entity(const std::pair<int,int> &pos, CHECK e_state);

public:

    void setPos(int x, int y);
    void setPos(const std::pair<int,int>& s);
    void setSTATE(CHECK state);
    int getX() const;
    int getY() const;
    CHECK getSTATE() const;

    std::pair<int,int> getPos() const;

    Entity& operator=(const Entity& e);

    bool operator==(const Entity& e);

    virtual bool move(const std::pair<int,int>& _p);

    virtual bool move(const std::pair<int,int>& _p, CHECK force_check);

    const std::pair<int, int> &getPreviousPos() const;

    void setPreviousPos(const std::pair<int, int> &previousPos);

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

    virtual void update(float time);

    int getId() const;

    void setId(int id);

    void resetSprite(float ScaleX, float ScaleY,
            float posX, float posY);

private:
    int x_;
    int y_;
    int id_;

    std::pair<int,int> previous_pos_;
    CHECK E_STATE;

    bool _canmove_;
public:
    bool isCanmove() const;

    void setCanmove(bool canmove);

public:
    sf::Sprite ENTITYSPRITE;
    float localtime;

    enum AnimType{
        SELECTED,
        MOVE,
        KILLED,
        NONE
    } astate;

};

/*
    E_STATE

    FORCE
    DEFAULT
    PIRATE
*/

static bool FORCEMOVE(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return curr.first >= 0 && curr.first < 13 && curr.second >= 0 && curr.second < 13;
};

static bool DEFAULT_MOVEMENT(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return ((abs(prev.first - curr.first) == 1 && abs(prev.second - curr.second) == 1) ||
    (abs(prev.first - curr.first) == 0 && abs(prev.second - curr.second) == 1) ||
    (abs(prev.first - curr.first) == 1 && abs(prev.second - curr.second) == 0))
    && FORCEMOVE(prev,curr);
};

static bool PIR_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return DEFAULT_MOVEMENT(prev,curr);
};

static bool FREGATH_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return DEFAULT_MOVEMENT(prev,curr) && ((curr.first % 12 == 0)
        || (curr.second % 12 == 0));
};

static bool ONSHIP(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return false;
};


static bool HORSE_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr)
{
    return (FORCEMOVE(prev,curr) &&
            ((abs(curr.first - prev.first)==1 && abs(curr.second - prev.second)==2)
             || (abs(curr.first - prev.first)==2 && abs(curr.second - prev.second)==1)));
};

static bool ARROW30_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return abs(prev.first - curr.first) == 1 && abs(prev.second - curr.second) == 0;
}

static bool ARROW31_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return abs(prev.second - curr.second) == 1 && abs(prev.first - curr.first) == 0;
}

static bool ARROW40_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return prev.second - curr.second == prev.first - curr.first &&
            abs(prev.second - curr.second) == 1 &&  abs(prev.first - curr.first) == 1;
}

static bool ARROW41_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return prev.second - curr.second != prev.first - curr.first &&
            abs(prev.second - curr.second) == 1 &&  abs(prev.first - curr.first) == 1;
}


static bool ARROW50_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (prev.first - curr.first == 1 && prev.second - curr.second == -1) ||
            (prev.first - curr.first == -1 && prev.second - curr.second == 0) ||
            (prev.first - curr.first == 0 && prev.second - curr.second == 1);
}
static bool ARROW51_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (prev.first - curr.first == 1 && prev.second - curr.second == 1) ||
           (prev.first - curr.first == -1 && prev.second - curr.second == 0) ||
           (prev.first - curr.first == 0 && prev.second - curr.second == -1);
}
static bool ARROW52_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (prev.first - curr.first == -1 && prev.second - curr.second == 1) ||
           (prev.first - curr.first == 1 && prev.second - curr.second == 0) ||
           (prev.first - curr.first == 0 && prev.second - curr.second == -1);
}
static bool ARROW53_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (prev.first - curr.first == -1 && prev.second - curr.second == -1) ||
           (prev.first - curr.first == 1 && prev.second - curr.second == 0) ||
           (prev.first - curr.first == 0 && prev.second - curr.second == 1);
}

static bool ARROW6_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (abs(prev.first - curr.first) == 1 && abs(prev.second - curr.second) == 0)
    || (abs(prev.first - curr.first) == 0 && abs(prev.second - curr.second) == 1);
}

static bool ARROW7_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return (abs(prev.first - curr.first) == 1 && abs(prev.second - curr.second) == 1);
}

static bool VERTEX_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return FORCEMOVE(prev,curr) && curr == prev;
}

static bool PLANE_STATE(const std::pair<int,int>& prev, const std::pair<int,int>& curr){
    return FORCEMOVE(prev,curr);
}


#endif //SHAKALAKA_SBASIC_H
