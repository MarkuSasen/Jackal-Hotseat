//
// Created by User on 19.07.2019.
//
#include <assert.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "Sbasic.h"


Entity::Entity() : x_(0), y_(0), previous_pos_(0,0), E_STATE(nullptr), ENTITYSPRITE(), id_(E_count++), astate(NONE), localtime(0.0f), _canmove_(false)
{}

Entity::Entity(const Entity& e) : x_(e.getY()), y_(e.getY()), E_STATE(e.getSTATE()), previous_pos_(e.getPreviousPos()), ENTITYSPRITE(), id_(e.getId()),
astate(e.astate), localtime(0.f), _canmove_(e.isCanmove())
{}

Entity::Entity(const std::pair<int,int> &pos, CHECK e_state) : x_(pos.first), y_(pos.second), E_STATE(e_state), previous_pos_(pos), ENTITYSPRITE(),
id_(E_count++), astate(NONE), localtime(0.f), _canmove_(false)
{}

void Entity::setPos(int x, int y)
{
    this->x_ = x;
    this->y_ = y;
}
void Entity::setPos(const std::pair<int,int>& s)
{
    x_ = s.first;
    y_ = s.second;
}

void Entity::setSTATE(CHECK state)
{
    E_STATE = state;
}
int Entity::getX() const
{
    return x_;
}
int Entity::getY() const
{
    return y_;
}
CHECK Entity::getSTATE() const
{
    return E_STATE;
}

std::pair<int,int> Entity::getPos() const
{
    return std::make_pair(x_,y_);
}

Entity& Entity::operator=(const Entity& e)
{
    setPos(e.getX(),e.getY());
    setSTATE(e.getSTATE());
    return *this;
}

bool Entity::operator==(const Entity& e)
{
    return this == &e;
}

bool Entity::move(const std::pair<int,int>& _p)
{
    assert(E_STATE != nullptr);
    if((*E_STATE)(getPos(),_p) && _canmove_)
    {
        setPreviousPos(getPos());
        setPos(_p);
        return true;
    } return false;
}

bool Entity::move(const std::pair<int,int>& _p, CHECK force_check)
{
    assert(force_check!= nullptr);
    if((*force_check)(getPos(),_p)/* && _canmove_*/)
    {
        setPreviousPos(getPos());
        setPos(_p);
        return true;
    } return false;
}

const std::pair<int, int> &Entity::getPreviousPos() const
{
    return previous_pos_;
}

void Entity::setPreviousPos(const std::pair<int, int> &previousPos)
{
    previous_pos_ = previousPos;
}

void Entity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //передаем положение сетки
    target.draw(ENTITYSPRITE,states);
}

int Entity::getId() const {
    return id_;
}

void Entity::setId(int id) {
    id_ = id;
}

void Entity::update(float time)
{
    //0b00001 = selected

}

void Entity::resetSprite(float ScaleX, float ScaleY, float posX, float posY) {
    ENTITYSPRITE.setScale(ScaleX, ScaleY);
    localtime = 0;
    //if(astate == SELECTED) astate = NONE;
}

bool Entity::isCanmove() const {
    return _canmove_;
}

void Entity::setCanmove(bool canmove) {
    _canmove_ = canmove;
}
