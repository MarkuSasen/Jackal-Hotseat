//
// Created by User on 03.08.2019.
//

#ifndef SHAKALAKA_GUI_H
#define SHAKALAKA_GUI_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <list>
#include <map>
#include <queue>
#include "SHAKAL.h"


class Pirate;
class Ship;


class GUI : public sf::Drawable
{
public:
    GUI();

    GUI(const GUI& gui);
    ~GUI();

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;


    //virtual void addcomponent_(const Component &_c);

    virtual void removecomponent_(sf::Drawable* _c);

    //virtual Component * getcomponent_(Drawable *);

    //virtual void clear();

    bool isHidden() const;
    void hide(bool h);

    const sf::Vector2f& getwrelativePos() const;
    const sf::Vector2f& getMousePos() const;
    const sf::FloatRect& getBounds() const;

    void setwrelativePos(const sf::Vector2f &_pos);
    void setMousePos(const sf::Vector2f& _pos);
    void setBounds(const sf::FloatRect& rect);
private:
    bool hidden;

    sf::Vector2f windowRelatedPos;
    sf::FloatRect globalBounds;
    sf::Vector2f localMousePosition;

protected:
    sf::View *camera;
    //std::list<Component> components;

public:
    //virtual void update(const sf::Vector2f& localMousePosition) = 0;
    virtual void setup() = 0;

};



class ShakalGui : public GUI
{
friend class ActionPanel;
friend class InventoryTab;
public:
    _CELL *_cell;

    enum GuiState{
        NOTHING = 0,
        ENDTURN = 1,
        PICK = 2,
        DROP = 3,
        SHIPPED,
        DESHIPPED,
        SELECTMODED,
        BACKGROUND
    } guiState;

    ShakalGui(sf::View *view, const std::map<std::string, sf::Texture *> &textures, _CELL *_cell);

    virtual GuiState update(const sf::Vector2f& localMousePosition);

    virtual void update(float time);

    virtual void setup();

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void move(const sf::Vector2f& vec);


    void loadActions(Pirate *pirate = nullptr, Ship *ship = nullptr);
    void unloadActions();


class ActionPanel : public sf::Drawable
        {
public:
    ActionPanel(Pirate *_pirate, ShakalGui &parent);
    ActionPanel(Ship *_ship, ShakalGui &parent);


    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

    void update(float time);

    Pirate *pirate; const ShakalGui& _parent;

    sf::Sprite DROP, PICKUP, DESHIP, SHIPED, SELECTMODE;
        } *_apanel;


class InventoryTab : public sf::Drawable{

public:
    InventoryTab(Pirate *_pir, ShakalGui &parent);
    InventoryTab(Ship *_ship, ShakalGui &parent);

    sf::Sprite TAB,GOLD_COIN,GALEON;
    sf::Sprite pirates[5];

    const ShakalGui& _parent;
    //bool hide_coin;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void update(float time);

} * _itab;

class InfoTab : public sf::Drawable{

    const ShakalGui& _parent;
    sf::Sprite player_rum, player_coins;
    sf::Text rum, coins;

public:
    InfoTab(ShakalGui& parent);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    void update(float time);

    void updateScore(int _score, int _rum);

    sf::Sprite InfoPanel, infobar;
    std::list<sf::Text> log;

    void updatelog(const char* s,...);
private:

} * _info;


private:

    sf::Vector2f scale;
    sf::Sprite background,
        End_turn_button;


    const std::map<std::string, sf::Texture*>& textures;
};



#endif //SHAKALAKA_GUI_H
