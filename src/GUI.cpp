#include <GUI.h>
//
// Created by User on 03.08.2019.
//

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <SFML/Graphics/Texture.hpp>
#include <chrono>
#include <cstdarg>
#include <SHAKAL.h>

#include "units.h"
#include "GUI.h"
#include "resources.h"


using namespace sf;
using namespace std;


GUI::GUI() : hidden(false),
        localMousePosition(0.f,0.f), globalBounds(0.f,0.f,0.f,0.f), windowRelatedPos(0.f,0.f)
{}

GUI::GUI(const GUI &gui) : hidden(gui.isHidden()), localMousePosition(gui.getMousePos()),
    globalBounds(gui.getBounds()), windowRelatedPos(gui.getwrelativePos())
{}

void GUI::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    /*if(!hidden)
        for(auto &e : components)
            target.draw(e,states);*/
}

/*void GUI::addcomponent_(const Component &_c) {
    components.push_back(_c);
}*/

void GUI::removecomponent_(sf::Drawable *_c) {
    //components.erase(std::find(components.begin(),components.end(),_c));
}

/*void GUI::clear() {
    components.clear();
}*/


/*Component * GUI::getcomponent_(sf::Drawable *comp) {
    return &*find_if(components.begin(),components.end(),[&comp](const Component &com)
    {
        return &const_cast<Component*>(&com)->getSprite() == dynamic_cast<Sprite*>(comp);
    });
}*/


GUI::~GUI() {
}

bool GUI::isHidden() const {
    return hidden;
}

void GUI::hide(bool h) {
    hidden = h;
}

const sf::Vector2f &GUI::getwrelativePos() const {
    return windowRelatedPos;
}

const sf::Vector2f &GUI::getMousePos() const {
    return localMousePosition;
}

const sf::FloatRect &GUI::getBounds() const {
    return globalBounds;
}

void GUI::setwrelativePos(const sf::Vector2f &_pos) {
    windowRelatedPos = _pos;
}

void GUI::setMousePos(const sf::Vector2f &_pos) {
    localMousePosition = _pos;
}

void GUI::setBounds(const sf::FloatRect &rect) {
    globalBounds = rect;
}




///////////////////////////////
///////////////////////////////
///////////////////////////////

ShakalGui::ShakalGui(sf::View *view, const std::map<std::string, sf::Texture *> &textures, _CELL *cell)
        : GUI(), scale(0.f, 0.f), _apanel(nullptr), _itab(nullptr),
          textures(textures),_cell(cell)
{
    camera = view;
}

ShakalGui::GuiState ShakalGui::update(const sf::Vector2f &localMousePosition) {

    if (End_turn_button.getGlobalBounds().contains(localMousePosition))
        return ENDTURN;
    else if(_apanel && _apanel->PICKUP.getGlobalBounds().contains(localMousePosition))
        return PICK;
    else if(_apanel && _apanel->DROP.getGlobalBounds().contains(localMousePosition))
        return DROP;
    else if(_apanel && _apanel->DESHIP.getGlobalBounds().contains(localMousePosition))
        return DESHIPPED;
    else if(_apanel && _apanel->SHIPED.getGlobalBounds().contains(localMousePosition))
        return SHIPPED;
    else if(_apanel && _apanel->SELECTMODE.getGlobalBounds().contains(localMousePosition))
        return SELECTMODED;
    return NOTHING;
}

void ShakalGui::setup()
{
    background.setTexture(*textures.at("ui.png"));
    background.setScale(camera->getSize().x/background.getTextureRect().width,
            camera->getSize().y/background.getTextureRect().height);
    background.setOrigin(background.getLocalBounds().width/2,
            background.getLocalBounds().height/2);
    background.setPosition(camera->getCenter());
    setwrelativePos(background.getPosition());

    End_turn_button.setTexture(*textures.at("move.png"));
    End_turn_button.setOrigin(End_turn_button.getLocalBounds().width/2,
            End_turn_button.getLocalBounds().height/2);
    End_turn_button.setPosition(background.getGlobalBounds().left + 1275,
            background.getGlobalBounds().top + 732);
    End_turn_button.setScale(1.150f, 0.85f);

    _info = new InfoTab(*this);
}


void ShakalGui::update(float time) {
    background.setPosition(camera->getCenter());
    End_turn_button.setPosition(background.getGlobalBounds().left + 1275,
                                background.getGlobalBounds().top + 732);


    if(_apanel)
        _apanel->update(time);
    if(_itab)
        _itab->update(time);

    _info->update(time);

}

void ShakalGui::draw(sf::RenderTarget &target, sf::RenderStates states) const {

    target.draw(_info->infobar);
    target.draw(_info->InfoPanel);

    target.draw(background);
    target.draw(End_turn_button);

    target.draw(*_info,states);

    if(_itab)
        target.draw(*_itab, states);

    if(_apanel)
        target.draw(*_apanel,states);
}

void ShakalGui::move(const sf::Vector2f &vec) {

}

void ShakalGui::loadActions(Pirate *pirate, Ship* ship) {

    if(pirate) {
        if (!_itab) delete _itab;

        _itab = new InventoryTab(pirate, *this);

        if (!_apanel) delete _apanel;

        _apanel = new ActionPanel(pirate, *this);
    }
    else if(ship){
        if (_itab) delete _itab;

        _itab = new InventoryTab(ship, *this);

        if (!_apanel) delete _apanel;

        _apanel = new ActionPanel(ship, *this);
    }
}

void ShakalGui::unloadActions() {
    delete _itab;
    _itab = nullptr;
    delete _apanel;
    _apanel = nullptr;
}


///////////////////////////////
///////////////////////////////
///////////////////////////////

ShakalGui::ActionPanel::ActionPanel(Pirate *_pirate, ShakalGui &parent) :
pirate(_pirate), _parent(parent){
    PICKUP.setTexture(*_parent.textures.at("pickup.png"));
    PICKUP.setOrigin(PICKUP.getLocalBounds().width/2,
                            PICKUP.getLocalBounds().height/2);
    PICKUP.setPosition(_parent.background.getGlobalBounds().left + 1363,
                              _parent.background.getGlobalBounds().top + 732);
    PICKUP.setScale(1.150f, 0.85f);

    DROP.setTexture(*_parent.textures.at("drop.png"));
    DROP.setOrigin(DROP.getLocalBounds().width/2,
                          DROP.getLocalBounds().height/2);
    DROP.setPosition(_parent.background.getGlobalBounds().left + 1450,
                            _parent.background.getGlobalBounds().top + 732);
    DROP.setScale(1.150f, 0.85f);


    if(_pirate->Player->getShip()->isOnShip(_pirate) && _pirate->isCanmove())
    {
        DESHIP.setTexture(*_parent.textures.at("ship_red.png"));
        DESHIP.setOrigin(SHIPED.getLocalBounds().width/2,
                         SHIPED.getLocalBounds().height/2);
        DESHIP.setPosition(_parent.background.getGlobalBounds().left + 1325,
                           _parent.background.getGlobalBounds().top + 760);
        DESHIP.setScale(0.20f, 0.20f);
    }else if(
            (abs(_pirate->Player->getShip()->getPos().first -
                    _pirate->getPos().first) <= 1 &&
                    abs(_pirate->Player->getShip()->getPos().second -
                        _pirate->getPos().second) <= 1) && _pirate->isCanmove()
            ){
        SHIPED.setTexture(*_parent.textures.at("ship_blue.png"));
        SHIPED.setOrigin(SHIPED.getLocalBounds().width/2,
                         SHIPED.getLocalBounds().height/2);
        SHIPED.setPosition(_parent.background.getGlobalBounds().left + 1363,
                           _parent.background.getGlobalBounds().top + 800);
        SHIPED.setScale(0.20f, 0.20f);
    }

    if(((*_parent._cell)[pirate->getPos()].getTiletype() == LIGHTHOUSE
        || (*_parent._cell)[pirate->getPos()].getTiletype() == EARTHQUAKE
        || (*_parent._cell)[pirate->getPos()].getTiletype() == CAVE)
            && pirate->isCanmove() && !pirate->Player->getShip()->isCanmove())
    {
        SELECTMODE.setTexture(*_parent.textures.at("lighthouse.png"));
        SELECTMODE.setOrigin(SHIPED.getLocalBounds().width/2,
                         SHIPED.getLocalBounds().height/2);
        SELECTMODE.setPosition(_parent.background.getGlobalBounds().left + 1233,
                           _parent.background.getGlobalBounds().top + 760);
        SELECTMODE.setScale(0.20f, 0.16f);
    }
}

void ShakalGui::ActionPanel::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(PICKUP);
    target.draw(DROP);
    target.draw(SHIPED);
    target.draw(DESHIP);
    target.draw(SELECTMODE);
}

void ShakalGui::ActionPanel::update(float time) {
    PICKUP.setPosition(_parent.background.getGlobalBounds().left + 1363,
                              _parent.background.getGlobalBounds().top + 732);
    DROP.setPosition(_parent.background.getGlobalBounds().left + 1450,
                            _parent.background.getGlobalBounds().top + 732);
    SHIPED.setPosition(_parent.background.getGlobalBounds().left + 1363,
                       _parent.background.getGlobalBounds().top + 800);
    DESHIP.setPosition(_parent.background.getGlobalBounds().left + 1325,
                       _parent.background.getGlobalBounds().top + 760);
    SELECTMODE.setPosition(_parent.background.getGlobalBounds().left + 1233,
                           _parent.background.getGlobalBounds().top + 760);
}

ShakalGui::ActionPanel::ActionPanel(Ship *_ship, ShakalGui &parent) :
_parent(parent){

}


///////////////////////////////
///////////////////////////////
///////////////////////////////

ShakalGui::InventoryTab::InventoryTab(Pirate *_pir, ShakalGui &parent) : _parent(parent){

    TAB.setTexture(*parent.textures.at("panel.png"));
    TAB.setOrigin(TAB.getLocalBounds().width/2,
                      TAB.getLocalBounds().height/2);
    TAB.setPosition(_parent.background.getGlobalBounds().left + 1094,
                        _parent.background.getGlobalBounds().top + 802);
    TAB.setScale(0.88f,0.88f);

    if(_pir->getInventory().size()) {
        if((*_pir->getInventory().begin())->getType() == 1) {
            GOLD_COIN.setTexture(*_parent.textures.at("coin.png"));
            GOLD_COIN.setOrigin(GOLD_COIN.getLocalBounds().width / 2,
                                GOLD_COIN.getLocalBounds().height / 2);
            GOLD_COIN.setScale(1.2f, 0.8f);
            GOLD_COIN.setPosition(TAB.getGlobalBounds().left + 40,
                                  TAB.getGlobalBounds().top + 55);
        }else {
            GALEON.setTexture(*_parent.textures.at("coin_c.png"));
            GALEON.setOrigin(GALEON.getLocalBounds().width / 2,
                             GALEON.getLocalBounds().height / 2);
            GALEON.setScale(1.2f, 0.8f);
            GALEON.setPosition(TAB.getGlobalBounds().left + 40,
                               TAB.getGlobalBounds().top + 55);
        }
    }
}

void ShakalGui::InventoryTab::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(TAB,states);
    target.draw(GOLD_COIN,states);
    target.draw(GALEON,states);
    for(int i = 0; i < 5; i++)
        target.draw(pirates[i]);
}

void ShakalGui::InventoryTab::update(float time) {

    TAB.setPosition(_parent.background.getGlobalBounds().left + 1094,
                        _parent.background.getGlobalBounds().top + 802);

    GOLD_COIN.setPosition(TAB.getGlobalBounds().left + 40,
                              TAB.getGlobalBounds().top + 55);

    GALEON.setPosition(TAB.getGlobalBounds().left + 40,
                       TAB.getGlobalBounds().top + 55);

    for(int i = 0; i < 5; i++)
        pirates[i].setPosition(TAB.getGlobalBounds().left + 40,
                               TAB.getGlobalBounds().top + 55 + 59*i);
}

ShakalGui::InventoryTab::InventoryTab(Ship *_ship, ShakalGui &parent) :
_parent(parent){
    TAB.setTexture(*parent.textures.at("panel.png"));
    TAB.setOrigin(TAB.getLocalBounds().width/2,
                  TAB.getLocalBounds().height/2);
    TAB.setPosition(_parent.background.getGlobalBounds().left + 1094,
                    _parent.background.getGlobalBounds().top + 802);
    TAB.setScale(0.88f,0.88f);

    char i = 0;
    for(auto &e: _ship->getPirates())
    {
        pirates[i].setTexture(*parent.textures.at("pirate_red.png"));
        pirates[i].setOrigin(pirates[i].getLocalBounds().width / 2,
                             pirates[i].getLocalBounds().height / 2);
        pirates[i].setPosition(TAB.getGlobalBounds().left + 40,
                               TAB.getGlobalBounds().top + 55 + 59*i);
        pirates[i].setScale(2.3f, 2.3f);

        ++i;
    }
}

//////////////////////////////////////
//////////////////////////////////////
/////////////////////////////////////

ShakalGui::InfoTab::InfoTab(ShakalGui &parent) : _parent(parent) {
    player_rum.setTexture(*_parent.textures.at("rum_icon.png"));
    player_rum.setOrigin(player_rum.getLocalBounds().width/2,
                         player_rum.getLocalBounds().height/2);
    player_rum.setPosition(_parent.background.getGlobalBounds().left + 22,
                           _parent.background.getGlobalBounds().top + 15);
    player_rum.setScale(0.6f, 0.6f);

    player_coins.setTexture(*_parent.textures.at("coin_icon.png"));
    player_coins.setOrigin(player_coins.getLocalBounds().width/2,
                           player_coins.getLocalBounds().height/2);
    player_coins.setPosition(_parent.background.getGlobalBounds().left + 190,
                             _parent.background.getGlobalBounds().top + 15);
    player_coins.setScale(0.6f, 0.6f);

    rum.setFont(fonts->coins_);
    rum.setCharacterSize(20);
    rum.setFillColor(Color::Black);
    rum.setPosition(player_rum.getPosition().x + 20, player_rum.getPosition().y - 6);

    coins.setFont(fonts->coins_);
    coins.setCharacterSize(20);
    coins.setFillColor(Color::Black);
    coins.setPosition(player_coins.getPosition().x + 20, player_rum.getPosition().y - 6);

    InfoPanel.setTexture(*_parent.textures.at("paper.png"));
    InfoPanel.setScale(0.55f,0.35f);
    InfoPanel.setPosition(_parent.background.getGlobalBounds().left + 20,_parent.background.getGlobalBounds().top + 680);

    infobar.setTexture(*_parent.textures.at("paper.png"));
    infobar.setScale(3.0f,0.05f);
    infobar.setPosition(_parent.background.getGlobalBounds().left + 2,_parent.background.getGlobalBounds().top);
}

void ShakalGui::InfoTab::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    target.draw(player_coins);
    target.draw(player_rum);
    target.draw(rum);
    target.draw(coins);

    for(auto &e : log){
        target.draw(e);

    }
}

void ShakalGui::InfoTab::update(float time) {
    player_rum.setPosition(_parent.background.getGlobalBounds().left + 22,
                           _parent.background.getGlobalBounds().top + 15);
    player_coins.setPosition(_parent.background.getGlobalBounds().left + 190,
                             _parent.background.getGlobalBounds().top + 15);
    coins.setPosition(player_coins.getPosition().x + 20, player_rum.getPosition().y - 6);
    rum.setPosition(player_rum.getPosition().x + 20, player_rum.getPosition().y - 6);
    InfoPanel.setPosition(_parent.background.getGlobalBounds().left + 20,_parent.background.getGlobalBounds().top + 680);
    infobar.setPosition(_parent.background.getGlobalBounds().left + 2,_parent.background.getGlobalBounds().top);


    if(log.begin() == log.end()) return;

    float height = log.begin()->getLocalBounds().height;
    log.begin()->setPosition(InfoPanel.getPosition().x + 5,
                             InfoPanel.getPosition().y + 5);
    int c = 0;
    for(auto e = log.begin(); e != log.end(); e++) {
        e->setPosition(log.begin()->getPosition().x,log.begin()->getPosition().y + height*c);
        ++c;
    }
}

void ShakalGui::InfoTab::updateScore(int _score, int _rum)
{
    coins.setString(std::to_string(_score));
    rum.setString(std::to_string(_rum));
}

void ShakalGui::InfoTab::updatelog(const char* s,...) {

    string act;
    std::stringstream stroke;

    va_list args;\
    va_start (args, s);

    for(char* i = (char*) s; *i != '\0'; i++)
    {
        if(*i == '%')
        {
            switch (*(i+1))
            {
                case 'd': stroke << va_arg(args, int); i++;
                    break;
                case 's': stroke << va_arg(args,const char*); i++;
                    break;
                case 'f': stroke << va_arg(args, double); i++;
                    break;
                case 'c': stroke << (char) va_arg(args, int); i++;
                    break;
                default: stroke << *i;
                    break;
            }
        }
        else
            stroke << *i;
    }


    sf::Text l;
    l.setFont(fonts->coins_);
    l.setFillColor(Color::Black);

    l.setString(stroke.str());
    l.setCharacterSize(20);
    l.setPosition(InfoPanel.getPosition().x + 5,
            InfoPanel.getPosition().y + 5);

    float height = l.getLocalBounds().height;


    for(auto &e : log) {
        e.move(0, height);
        e.setFillColor(Color::Black);
    }

    log.push_front(l);

    (*log.begin()).setFillColor(Color(35, 78, 145, 255));
    if(log.size() > 7) log.pop_back();
}


