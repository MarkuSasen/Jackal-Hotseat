//
// Created by User on 21.07.2019.
//
#include <algorithm>
#include <iostream>
#include <numeric>
#include "SHAKAL.h"
#include "player.h"
#include "GUI.h"
#include "resources.h"
using namespace sf;
using namespace std;

static _CELL* cell = nullptr;

static void LOCK(Pirate* pir)
{
    for (auto &e : pir->Player->getPirates())
        e->setCanmove(false);
    pir->Player->getShip()->setCanmove(false);
}

SHAKAL::SHAKAL(int players) : loadedtextures(false), mainview(), TURN(0), //texture_prefub(&textures->textures_)
players_(players)
{

    for(int i = 0; i<players; i++)
        PLAYERS[i] = new player(i, textures->textures_);

    cell = new _CELL(*this);
}




int SHAKAL::run(sf::RenderWindow& window) {

    float wSizeX = window.getSize().x;
    float wSizeY = window.getSize().y;
    Transform grid_transform = Transform().translate(215.f,149.f);

////////////////////////////////////

///////////////////////////////////
    struct ViewIntersect
    {
        sf::IntRect rect;
    }VUP, VDOWN, VRIGHT, VLEFT;

    unsigned int rectstate = 0b0000;
    //0bxxx1 - up
    //0bxx1x - down
    //0bx1xx - right
    //0b1xxx - left
    VUP = {IntRect(Vector2i(10,10),Vector2i(wSizeX - 10.f,50.f))};
    VDOWN = {IntRect(Vector2i(10, wSizeY - 50.f), Vector2i(wSizeX - 10.f, wSizeY - 10.f))};
    VLEFT = {IntRect(Vector2i(10, 10), Vector2i(50, wSizeY - 10))};
    VRIGHT = {IntRect(Vector2i(wSizeX-50, 10), Vector2i(wSizeX - 10.f, wSizeY - 10.f))};
////////////////////////////////////



////////////////////////////////////

    Font f;
    f.loadFromFile("font.ttf");

/////
    Text mousePos;
    Text Prevcoord("", f), newcoord("",f);
/////

    //mousePos.setScale(1,1);
    mousePos.setCharacterSize(30);
    mousePos.setFont(f);
    bool running = true;
    bool scaled = false;
    //loading textures
    Sprite backgr;
    //Sprite ENDTURN;

        sf::FloatRect VIEW(0.f,0.f,wSizeX,wSizeY);
        mainview.reset(VIEW);

    if (!textures->textures_.empty()) {
        backgr.setTexture(*textures->textures_["map.png"], true);

        backgr.setTextureRect(IntRect(470,100,1435,1325));
        backgr.setScale(textures->textures_["background.png"]->getSize().x / wSizeX,
                        textures->textures_["background.png"]->getSize().y / wSizeY);

//        ENDTURN.setTexture(*texture_prefub["ricardo.png"]);
//        ENDTURN.move(80,80);
//        ENDTURN.scale(0.5f,0.5f);
//        ENDTURN.setOrigin(ENDTURN.getLocalBounds().width/2,ENDTURN.getLocalBounds().height/2);
    } else
        return -1;
//////////////////////////////////
    Pirate* s_pirate = nullptr;
    Ship *s_ship = nullptr;
/////////////////////////////////



    player* you = PLAYERS[TURN%players_];
    for(auto &e : you->getPirates())
        you->getPirateById(e->getId())->setCanmove(true);
    if(!you->getShip()->getPirates().empty())you->getShip()->setCanmove(true);

////////////////////////////////
  View mainView(window.getDefaultView());
  bool checkView = true, ViewBounds = true, locked = false, smode = false;
  int lig = 5;
//////////////////////////////
  float currentFrame = 0;
  Clock clock;
///////////////////////////
    ShakalGui gui(&mainView, textures->textures_, cell);
    gui.setup();
    gui._info->updateScore(you->getScore(),you->getRum());
///////////////////////////

    std::vector<std::pair<int,int>> tempor;
///////////////////////////
    while (true) {

        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;

        if(running == false)
        {
            float shutdown = 0;
            sf::Text text;
            text.setFillColor(Color::Black);
            text.setFont(fonts->coins_);
            text.setString("Congratulation! Player " + std::to_string((TURN - 1)%players_) + " won!");
            text.setOrigin(text.getLocalBounds().width/2,text.getLocalBounds().height/2);
            text.setPosition(mainView.getCenter());
            text.setCharacterSize(50);
            while(shutdown < 1000.0f) {
                shutdown += clock.getElapsedTime().asSeconds();
                window.draw(backgr);
                cell->draw(window, RenderStates(grid_transform));
                window.draw(mousePos);
                window.draw(gui);
                window.draw(text);
                window.display();
            }
            return -1;
        }


        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return -1;
            } else if (event.type == Event::Resized) {
                //backgr.setScale(wSizeX / texture_prefub["background.png"]->getSize().x, wSizeY / texture_prefub["background.png"]->getSize().y);
            }

                //ОТСЛЕЖИВАНИЕ КЛИКОВ МЫШКОЙ
            else if (event.type == Event::MouseButtonPressed) {
                Vector2i sa(event.mouseButton.x, event.mouseButton.y);
                Vector2f mouse = window.mapPixelToCoords(sa, mainView);
                /*mousePos.setString(std::string("X: ").append(to_string(mouse.x)).append(", Y: ").
                        append(to_string(mouse.y)));*/

                switch(gui.update(mouse)) {
                    //ЗАКАНЧИВАЕМ ХОД?
                    case 1 :
                        {
                             bool end = true;
                             for (auto &e : you->getPirates()) {
                                 if(e->isCanmove() || you->getShip()->isCanmove())
                                     end = false;
                             }

                             if(end) {
                                 you = PLAYERS[(++TURN) % players_];

                                 for (auto &e : you->getPirates()) {
                                     you->getPirateById(e->getId())->setCanmove(true);
                                 }
                                 if(!you->getShip()->getPirates().empty())you->getShip()->setCanmove(true);


                                 for (auto &e : cell->getCoordCell()) {
                                     e.second.A->update();
                                 }

//                                 if (s_pirate) {
//                                     s_pirate->astate = Entity::AnimType::NONE;
//                                     s_pirate->resetSprite(2.3f, 2.3f, 0.0f, 0.0f);
//                                     s_pirate = nullptr;
//                                 }
                                 gui.unloadActions();
                                 gui._info->updateScore(you->getScore(), you->getRum());
                                 if (checkScore())
                                     running = false;

                                 gui._info->updatelog("TURN %d", TURN);
                             }
                            continue;

                            break;
                        }
                        //PICK ITEM
                    case 2:
                        {
                            if(s_pirate) {
                                s_pirate->pickup(cell->getCoordCell()[s_pirate->getPos()]);
                                gui.loadActions(s_pirate);
//                                gui._info->updatelog("PICKED an item %s", s_pirate->getInventory().front()->_type==GameItem::ItemType::GALEON?
//                                                                          "GALEON":"GOLD");
                            }

                            continue;
                            break;
                        }
                    //DROP
                    case 3:
                    {
                        if(s_pirate) {
                            s_pirate->drop(cell->getCoordCell()[s_pirate->getPos()]);
                            gui.loadActions(s_pirate);
                        }
                        continue;
                            break;
                    }
                    //GO SHIP
                    case 4:
                    {
                        if(s_pirate)
                        {
                            s_pirate->Player->getShip()->add(s_pirate);
                            //ЗАПРЕЩАЕМ ХОДИТЬ ВСЕМ ПИРАТАМ ЭТОГО ИГРОКА
                            LOCK(s_pirate);
                            gui._info->updateScore(you->getScore(),you->getRum());

                            cell->getCoordCell()[s_pirate->getPreviousPos()].removePir(s_pirate);
                        }
                        continue;
                            break;
                    }
                    //LAND
                    case 5:
                    {
                        if(s_pirate)
                        {
                            s_pirate->Player->getShip()->remove(s_pirate);
                            //ЗАПРЕЩАЕМ ХОДИТЬ ВСЕМ ПИРАТАМ ЭТОГО ИГРОКА
                            LOCK(s_pirate);

                            cell->getCoordCell()[s_pirate->getPos()]._do(s_pirate);
                        }
                        continue;
                            break;
                    }
                    case 6:
                    {
                        if(s_pirate)
                        {
                            if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == LIGHTHOUSE) {
                                smode = true;
                                lig = 4;
                            }else if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == EARTHQUAKE)
                            {
                                smode = true;
                                lig = 2;
                            }else if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == CAVE)
                            {
                                smode = true;
                                lig = 1;
                            }
                        }
                        continue;
                            break;
                    }
                    default: break;
                }


                //ПРОВЕРКА ПЕРЕМЕЩЕНИЯ ПИРАТА
                bool moved = false;
                try {
                    Tile &t = cell->getCoordCell().at(make_pair((mouse.x - 163) / 104,
                                                                (mouse.y - 96) / 104));
                    //cout << t.getTiletype() << " ";
                    if (s_pirate && !smode) {

                        //может ли пойти на эту клетку?

                        if (s_pirate->astate == Entity::AnimType::SELECTED)
                            moved = s_pirate->move(make_pair((mouse.x - 163) / 104, (mouse.y - 96) / 104));

                        if (moved) {
                            sounds->play(_sounds_::Sounds::MOVE);
                            s_pirate->astate = Entity::AnimType::MOVE;
                            s_pirate->resetSprite(2.3f, 2.3f, 0, 0);

                            //ПРОВЕРКА НА ПЕРЕСЕЧЕНИЯ
                            cell->getCoordCell()[s_pirate->getPos()].removePir(s_pirate);
                            //checkWhat(s_pirate->getPos(),t.getTiletype(),s_pirate);
                            //ЗАПРЕЩАЕМ ХОДИТЬ ВСЕМ ПИРАТАМ ЭТОГО ИГРОКА

                            //ПРИМЕНЕНИЕ ДЕЙСТВИЯ КЛЕТКИ
                            t._do(s_pirate);
                            //LOCK(s_pirate,false);
                            //ВОСПРОИЗВЕДЕНИЕ ЗВУКА

                            s_pirate = nullptr;
                            gui.unloadActions();
                        }
                    } else if(s_ship && !smode)
                    {
                        moved = s_ship->move(make_pair((mouse.x - 163) / 104, (mouse.y - 96) / 104));
                        if(moved)
                        {
                            for( auto &e : t.getPirates())
                            {
                                if(e->Player->getID() != you->getID())
                                    t.removePir(e);
                                else
                                    s_ship->add(e);
                            }

                            for(auto &e : you->getPirates())
                                e->setCanmove(false);
                                s_ship->setCanmove(false);
                            s_ship = nullptr;
                            gui.unloadActions();
                        }
                        else cout << s_ship->isCanmove() << " " << s_ship->getSTATE();
                    }
                    else if(smode && s_pirate)
                    {

                        if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == LIGHTHOUSE) {
                            t.show();
                            //cout << t.getTiletype() << " showed\n";
                        }else if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == EARTHQUAKE)
                        {
                            if(t.hasCoins() || t.hasTreasure() || !t.getPirates().empty() || t.A->used)
                                lig++;
                            else {
                                tempor.push_back(make_pair((mouse.x - 163) / 104,
                                                           (mouse.y - 96) / 104));
                                cell->getCoordCell()[make_pair((mouse.x - 163) / 104,
                                        (mouse.y - 96) / 104)].sprite.setTexture(*textures->textures_["close.png"]);
                            }
                        } else if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == CAVE)
                        {
                            if(t.getTiletype() == CAVE) {
                                if(cell->getCoordCell()[make_pair((mouse.x - 163) / 104,(mouse.y - 96) / 104)].getPirates().empty())
                                    tempor.push_back(make_pair((mouse.x - 163) / 104,
                                                           (mouse.y - 96) / 104));
                                else if((*cell->getCoordCell()[make_pair((mouse.x - 163) / 104,(mouse.y - 96) / 104)].getPirates().begin())->Player->getID()
                                                == s_pirate->Player->getID())
                                    tempor.push_back(make_pair((mouse.x - 163) / 104,
                                                               (mouse.y - 96) / 104));
                            }
                            else lig++;
                        }

                        lig--;
                        if(lig == 0)
                        {
                            smode = false;
                            s_pirate->setCanmove(false);
                            s_pirate->setSTATE(PIR_STATE);
                            if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == EARTHQUAKE){
                                //std::swap(cell->getCoordCell()[tempor[0]],cell->getCoordCell()[tempor[1]]);
                                delete cell->getCoordCell()[tempor[0]].A;
                                delete cell->getCoordCell()[tempor[1]].A;

                                TileType te = cell->getCoordCell()[tempor[0]].getTiletype();
                                cell->getCoordCell()[tempor[0]].setTiletype(cell->getCoordCell()[tempor[1]].getTiletype());
                                cell->getCoordCell()[tempor[1]].setTiletype(te);

                                cell->setDefaultTileSprite(cell->getCoordCell()[tempor[0]]);
                                cell->setDefaultTileSprite(cell->getCoordCell()[tempor[1]]);

                                tempor.clear();
                            }else if(cell->getCoordCell()[s_pirate->getPos()].getTiletype() == CAVE)
                                s_pirate->move(tempor[0], FORCEMOVE);
                        }
                        continue;
                    }
                } catch (const std::out_of_range &exc) {}


                if (s_pirate) {
                    s_pirate->astate = Entity::AnimType::NONE;
                    s_pirate->resetSprite(2.3f, 2.3f, 0.0f, 0.0f);
                    s_pirate = nullptr;
                    gui.unloadActions();
                } else if(s_ship)
                {
                    s_ship = nullptr;
                    gui.unloadActions();
                }
            }
        }

////////////////////// Движение Камеры
        if (checkView) {
            const Vector2i mp(Mouse::getPosition(window));
            //const Vector2f globalmp(window.mapPixelToCoords(mp,mainView));



            if (VUP.rect.contains(mp.x, mp.y))
                rectstate = (rectstate | 0b0001);
            else if (VDOWN.rect.contains(mp.x, mp.y))
                rectstate = (rectstate | 0b0010);

            if (window.getView().getCenter().y - mainView.getSize().y / 2 <= backgr.getGlobalBounds().top)
                rectstate = rectstate & 0b1110;
            else if (window.getView().getCenter().y + mainView.getSize().y / 2 >= backgr.getGlobalBounds().height)
                rectstate = rectstate & 0b1101;

            float moveY = 2.f * ((rectstate << 2) ? (rectstate >> 1) ? 1 : -1 : 0);


            if (VRIGHT.rect.contains(mp.x, mp.y))
                rectstate = (rectstate | 0b0100);
            else if (VLEFT.rect.contains(mp.x, mp.y))
                rectstate = (rectstate | 0b1000);

            if (window.getView().getCenter().x - mainView.getSize().x / 2 <= backgr.getGlobalBounds().left)
                rectstate = rectstate & 0b0111;
            else if (window.getView().getCenter().x + mainView.getSize().x / 2 >= backgr.getGlobalBounds().width)
                rectstate = rectstate & 0b1011;

            float moveX = 2.f * ((rectstate >> 2) ? (rectstate >> 3) ? -1 : 1 : 0);


            if (rectstate && ViewBounds) {
                Vector2f movement(moveX, moveY);
                mainView.move(movement);
                gui.update(time);
            }

            rectstate = 0b0000;
        }
/////////////////////////////////////////////////
/////////////////////////////////////////////////Клавиатура

            if(!s_pirate) {
                //проверка на состояние пирата
                if (you->getPirates().size() >= 1 && sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
                    if (you->selectPirate(0)->astate != Entity::AnimType::MOVE) {
                        s_pirate = you->selectPirate(0);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (you->getPirates().size() >= 2 && sf::Keyboard::isKeyPressed(Keyboard::Num2)) {
                    if (you->selectPirate(1)->astate != Entity::AnimType::MOVE){
                        s_pirate = you->selectPirate(1);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (you->getPirates().size() >= 3 && sf::Keyboard::isKeyPressed(Keyboard::Num3)) {
                    if (you->selectPirate(2)->astate != Entity::AnimType::MOVE){
                        s_pirate = you->selectPirate(2);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (you->getPirates().size() >= 4 && sf::Keyboard::isKeyPressed(Keyboard::Num4)) {
                    if (you->selectPirate(3)->astate != Entity::AnimType::MOVE){
                        s_pirate = you->selectPirate(3);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (you->getPirates().size() >= 5 && sf::Keyboard::isKeyPressed(Keyboard::Num5)) {
                    if (you->selectPirate(4)->astate != Entity::AnimType::MOVE){
                        s_pirate = you->selectPirate(4);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (you->getPirates().size() == 6 && sf::Keyboard::isKeyPressed(Keyboard::Num6)) {
                    if (you->selectPirate(5)->astate != Entity::AnimType::MOVE){
                        s_pirate = you->selectPirate(5);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }
                } else if (sf::Keyboard::isKeyPressed(Keyboard::Q)) {
                    you->cscore();
                }

            }
            else
            {
                if (sf::Keyboard::isKeyPressed(Keyboard::Num1)) {
                    if (you->getPirates().size() >= 1 && you->selectPirate(0)->astate != Entity::AnimType::MOVE) {
                            s_pirate->astate=Entity::AnimType::NONE;
                            s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                        s_pirate = you->selectPirate(0);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        s_pirate->localtime = 0;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }

                } else if (sf::Keyboard::isKeyPressed(Keyboard::Num2)) {

                    if (you->getPirates().size() >= 2 && you->selectPirate(1)->astate != Entity::AnimType::MOVE) {
                            s_pirate->astate=Entity::AnimType::NONE;
                            s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                            s_pirate = you->selectPirate(1);
                            s_pirate->astate = Pirate::AnimType::SELECTED;
                            s_pirate->localtime = 0;
                            gui.loadActions(s_pirate);
                            s_ship = nullptr;

                    }

                } else if (sf::Keyboard::isKeyPressed(Keyboard::Num3)) {

                    if (you->getPirates().size() >= 3 && you->selectPirate(2)->astate != Entity::AnimType::MOVE) {
                            s_pirate->astate=Entity::AnimType::NONE;
                            s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                        s_pirate = you->selectPirate(2);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        s_pirate->localtime = 0;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }

                } else if (sf::Keyboard::isKeyPressed(Keyboard::Num4)) {

                    if (you->getPirates().size() >= 4 && you->selectPirate(3)->astate != Entity::AnimType::MOVE) {
                        s_pirate->astate=Entity::AnimType::NONE;
                        s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                        s_pirate = you->selectPirate(3);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        s_pirate->localtime = 0;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }

                } else if (sf::Keyboard::isKeyPressed(Keyboard::Num3)) {

                    if (you->getPirates().size() >= 5 && you->selectPirate(4)->astate != Entity::AnimType::MOVE) {
                        s_pirate->astate=Entity::AnimType::NONE;
                        s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                        s_pirate = you->selectPirate(4);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        s_pirate->localtime = 0;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }

                } else if (sf::Keyboard::isKeyPressed(Keyboard::Num3)) {

                    if (you->getPirates().size() == 6 && you->selectPirate(5)->astate != Entity::AnimType::MOVE) {
                        s_pirate->astate=Entity::AnimType::NONE;
                        s_pirate->resetSprite(2.3f,2.3f,0.f,0.f);

                        s_pirate = you->selectPirate(5);
                        s_pirate->astate = Pirate::AnimType::SELECTED;
                        s_pirate->localtime = 0;
                        gui.loadActions(s_pirate);
                        s_ship = nullptr;
                    }

                }
            }

            if(!s_ship)
            {
                if(Keyboard::isKeyPressed(Keyboard::Tilde))
                {
                    s_ship = you->getShip();
                    if(s_pirate && s_pirate->astate != Entity::AnimType::MOVE) {
                        s_pirate->astate = Entity::AnimType::NONE;
                        s_pirate->resetSprite(2.3f, 2.3f, 0.f, 0.f);
                        s_pirate = nullptr;
                    }
                    gui.loadActions(nullptr,s_ship);
                }
            }

///////////////////////////////////
        window.clear();


                window.draw(backgr);
                cell->draw(window, RenderStates(grid_transform));
                window.draw(mousePos);
        for(int i = 0; i < players_; i++){
                PLAYERS[i]->update(time);
                PLAYERS[i]->draw(window,RenderStates(grid_transform));
        }

                window.draw(Prevcoord);
                window.draw(newcoord);

        window.setView(mainView);

        window.draw(gui);

        window.display();

    }
    return -1;
}




//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

SHAKAL::~SHAKAL()
{
//    for(auto &e : texture_prefub){
//        delete(e.second);
//    }

    //delete(cell);
}




Tile& _CELL::setDefaultTileSprite(Tile& tile)
{
#define T(NAME) *textures->textures_[NAME]
#define sprite tile.sprite
sprite.setRotation(0.f);
    switch(tile.getTiletype())
    {
         case VOID : sprite.setTexture(T("close.png"));
            break;
         case SEA: //sprite.setTexture(T("sea.png"));
            tile.A = new SEA_ACTION(&tile,this);
            break;
         case CLEAR:
             sprite.setTexture(T(std::string("clear").append(to_string(
                     (std::chrono::system_clock::now().time_since_epoch().count()%4 ) + 1).append(".png"))));
             tile.A = new REGULAR_ACTION(&tile,this);
             break;
        case ARROW1: sprite.setTexture(T("arrow1.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW2: sprite.setTexture(T("arrow2.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW3: sprite.setTexture(T("arrow3.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW4: sprite.setTexture(T("arrow4.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW5: sprite.setTexture(T("arrow5.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW6: sprite.setTexture(T("arrow6.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case ARROW7: sprite.setTexture(T("arrow7.png"));
            tile.A = new ARROWS(&tile, this);
            break;
        case HORSES: sprite.setTexture(T("horses.png"));
            tile.A = new HORSE_ACTION(&tile,this);
            break;
        case TURNTABLE2: sprite.setTexture(T("turntable2.png"));
            tile.A = new VERTEX(&tile,2,this);
            break;
        case TURNTABLE3: sprite.setTexture(T("turntable3.png"));
            tile.A = new VERTEX(&tile,3,this);
            break;
        case TURNTABLE4: sprite.setTexture(T("turntable4.png"));
            tile.A = new VERTEX(&tile,4,this);
            break;
        case TURNTABLE5: sprite.setTexture(T("turntable5.png"));
            tile.A = new VERTEX(&tile,5,this);
            break;
        case ICE: sprite.setTexture(T("ice.png"));
            tile.A = new ICE_ACTION(&tile, this);
            break;
        case TRAP: sprite.setTexture(T("trap.png"));
            tile.A = new TRAP_ACTION(&tile,this);
            break;
        case CANNIBAL: sprite.setTexture(T("cannibal.png"));
            tile.A = new CANNIBAL_ACTION(&tile,this);
            break;
        case FORTRESS: sprite.setTexture(T("fortress.png"));
            tile.A = new CASTLE_ACTION(&tile,this);
            break;
        case COINS1: sprite.setTexture(T("coins1.png"));
            tile.A = new GOLD_ACTION(&tile,this,1);
            break;
        case COINS2: sprite.setTexture(T("coins2.png"));
            tile.A = new GOLD_ACTION(&tile,this,2);
            break;
        case COINS3: sprite.setTexture(T("coins3.png"));
            tile.A = new GOLD_ACTION(&tile,this,3);
            break;
        case COINS4: sprite.setTexture(T("coins4.png"));
            tile.A = new GOLD_ACTION(&tile,this,4);
            break;
        case COINS5: sprite.setTexture(T("coins5.png"));
            tile.A = new GOLD_ACTION(&tile,this,5);
            break;
        case TREASURE: sprite.setTexture(T("treasure.png"));
            tile.A = new GALEON_ACTION(&tile,this);
            break;
        case PLANE: sprite.setTexture(T("plane.png"));
            tile.A = new PLANE_ACTION(&tile,this);
            break;
        case BALOON: sprite.setTexture(T("ballon.png"));
            tile.A = new BALLOON_ACTION(&tile,this);
            break;
        case CANNON: sprite.setTexture(T("cannon0.png"));
            tile.A = new CANON_ACTION(&tile,this);
            break;
        case LIGHTHOUSE: sprite.setTexture(T("lighthouse.png"));
            tile.A = new BEACON_ACTION(&tile,this);
            break;
        case BEN: sprite.setTexture(T("ben.png"));
            tile.A = new ADDITIONAL_PIRATES(&tile,this);
            break;
        case MISSIONARY: sprite.setTexture(T("missionary.png"));
            tile.A = new ADDITIONAL_PIRATES(&tile,this);
            break;
        case FRIDAY: sprite.setTexture(T("friday.png"));
            tile.A = new ADDITIONAL_PIRATES(&tile,this);
            break;
        case RUM1: sprite.setTexture(T("rum1.png"));
            tile.A = new RUM_ACTION(&tile,this,1);
            break;
        case RUM2: sprite.setTexture(T("rum2.png"));
            tile.A = new RUM_ACTION(&tile,this,2);
            break;
        case RUM3: sprite.setTexture(T("rum3.png"));
            tile.A = new RUM_ACTION(&tile,this,3);
            break;
        case RUM_BARREL: sprite.setTexture(T("rum_barrel.png"));
            tile.A = new RUMBARREL_ACTION(&tile,this);
            break;
        case CAVE: sprite.setTexture(T("cave.png"));
            tile.A = new CAVE_ACTION(&tile,this);
            break;
        case EARTHQUAKE: sprite.setTexture(T("earthquake.png"));
            tile.A = new EARTHQUAKE_ACTION(&tile,this);
            break;
        case JUNGLE: sprite.setTexture(T("jungle.png"));
            tile.A = new JUNGLE_ACTION(&tile,this);
            break;
        case WEED: sprite.setTexture(T("weed.png"));
            tile.A = new WEED_ACTION(&tile,this, &instance);
            break;
        case BOAT: sprite.setTexture(T("boat.png"));
            tile.A = new REGULAR_ACTION(&tile,this);
            break;
        case KERNELS: sprite.setTexture(T("ice.png"));
            tile.A = new REGULAR_ACTION(&tile,this);
            break;
        case CART: sprite.setTexture(T("cart.png"));
            tile.A = new REGULAR_ACTION(&tile,this);
            break;
        case CROCODILE: sprite.setTexture(T("crocodile.png"));
            tile.A = new CROCO_ACTION(&tile,this);
            break;
        case ABORIGINAL: sprite.setTexture(T("aboriginal.png"));
            tile.A = new ABORIGINAL_ACTION(&tile,this);
            break;
        default: sprite.setTexture(T("fon.png"));
            tile.A = new REGULAR_ACTION(&tile,this);
            break;
    }
#undef T
    sprite.setOrigin(sprite.getLocalBounds().width/2,
                     sprite.getLocalBounds().height/2);
    //tile.setSprite(sprite);
#undef sprite

    return tile;
}

void SHAKAL::recalculate_cols()
{
    SHAKAL::collision_.friendlyPirates.clear();
    collision_.allPirates.clear();

    for(int i = 0; i < 2; i++)
        for(auto &e : PLAYERS[i]->getPirates())
            collision_.allPirates.insert(make_pair(e->getPos(),e));

        //прогоняемся по всем пиратам
   /* for(auto e = collision_.allPirates.begin(); e != collision_.allPirates.end(); ++e)
    {
        auto key = (*e).first;
        //если на 1 и той же клетке стоит больше чем 1 пират
        if(collision_.allPirates.count(key) > 1)
        {
            //парсим их в структуры
            auto range = collision_.allPirates.equal_range(key);
            for(auto i = range.first; i!= range.second; ++i)
            {
            }
            e = range.second;
        }
    }*/
}

void SHAKAL::proccessEvent(sf::RenderWindow &window) {

}

int SHAKAL::checkWhat(const std::pair<int, int> &pos, TileType type, Pirate *whom) {
    //0 - нету пересечений
    //1 - есть пересечения, но клетка позволяет им быть
    //2 - убить всех, кто на этой клетке

    bool safety_place = false;
    std::vector<Pirate*> on_this_place;


    for(int i = 0; i < players_; i++)
    {
        std::copy_if(PLAYERS[i]->getPirates().begin(),PLAYERS[i]->getPirates().end(),
                std::back_inserter(on_this_place), [&pos, &whom](Pirate* pir){
                    return pir->getPos() == pos && pir != whom;
        });

        if(pos == PLAYERS[i]->getShip()->getPos() && !PLAYERS[i]->isMyPir(whom)) {
            whom->astate = Pirate::AnimType::MOVE;
            whom->Player->kill(whom);
        }
    }



    for(auto &e : on_this_place)
    {

        if (e->Player->getID() != whom->Player->getID())
        {
            e->astate = Pirate::AnimType::MOVE;
            e->Player->kill(e);
        }

    }


}

int SHAKAL::checkScore() {

    std::vector<int> scores(players_);
    for(int i = 0; i < players_; i++)
        scores[i] = PLAYERS[i]->getScore();

    sort(scores.begin(), scores.end());

    int max_score = *std::max_element(scores.begin(), scores.end());
    int sum = std::accumulate(scores.begin(),scores.end(),0);
    int scnd = scores[players_-2];

    //cout << "sum, max, scnd = " << sum << " " << max_score << " " << scnd << "\n";

    if(37 - sum + scnd <= max_score)
        return 1;

    return 0;
}

SHAKAL::_collisions SHAKAL::_collisions::update(Pirate *mov) {

    if (allPirates.count(mov->getPos()) >= 1) {
        auto range = allPirates.equal_range(mov->getPos());
        for (auto i = range.first; i != range.second; ++i) {
            if ((*i).second->Player->getID() != mov->Player->getID()) {
                //kill
                i->second->Player->kill(i->second);
            }
        }
    }
    return *this;
}


_CELL::_CELL(SHAKAL& instance) : coord_cell(), offsetX(6.f), offsetY(6.f), globalY(65.f), globalX(80.f), scaleX(0.25), scaleY(0.25),
instance(instance)
{
    int X_count = 13, Y_count = 13;
    std::vector<Tile> shuffle(169, Tile(SEA));
    //shuffle.reserve(169);

    auto e = std::fill_n(shuffle.begin(),18, Tile(CLEAR));
    e = std::fill_n(e,2, Tile(HORSES));
    e = std::fill_n(e,5, Tile(TURNTABLE2));
    e = std::fill_n(e,4, Tile(TURNTABLE3));
    e = std::fill_n(e,2, Tile(TURNTABLE4));
    e = std::fill_n(e,1, Tile(TURNTABLE5));
    e = std::fill_n(e,6, Tile(ICE));
    e = std::fill_n(e,3, Tile(CAVE));
    e = std::fill_n(e,1, Tile(CANNIBAL));
    e = std::fill_n(e,2, Tile(FORTRESS));
    e = std::fill_n(e,5, Tile(COINS1));
    e = std::fill_n(e,5, Tile(COINS2));
    e = std::fill_n(e,3, Tile(COINS3));
    e = std::fill_n(e,2, Tile(COINS4));
    e = std::fill_n(e,1, Tile(COINS5));
    e = std::fill_n(e,1, Tile(TREASURE));
    e = std::fill_n(e,1, Tile(PLANE));
    e = std::fill_n(e,2, Tile(BALOON));
    e = std::fill_n(e,1, Tile(LIGHTHOUSE));
    e = std::fill_n(e,1, Tile(BEN));
    e = std::fill_n(e,1, Tile(MISSIONARY));
    e = std::fill_n(e,1, Tile(FRIDAY));
    e = std::fill_n(e,3, Tile(RUM1));
    e = std::fill_n(e,2, Tile(RUM2));
    e = std::fill_n(e,1, Tile(RUM3));
    e = std::fill_n(e,4, Tile(RUM_BARREL));
    e = std::fill_n(e,1, Tile(EARTHQUAKE));
    e = std::fill_n(e,3, Tile(JUNGLE));
    e = std::fill_n(e,1, Tile(ABORIGINAL));
    e = std::fill_n(e,4, Tile(CROCODILE));
    e = std::fill_n(e,3, Tile(ARROW1));
    e = std::fill_n(e,3, Tile(ARROW2));
    e = std::fill_n(e,3, Tile(ARROW3));
    e = std::fill_n(e,3, Tile(ARROW4));
    e = std::fill_n(e,3, Tile(ARROW5));
    e = std::fill_n(e,3, Tile(ARROW6));
    e = std::fill_n(e,3, Tile(ARROW7));
    e = std::fill_n(e,52, Tile(SEA));
    e = std::fill_n(e,3, Tile(TRAP));
    e = std::fill_n(e,2, Tile(WEED));
    e = std::fill_n(e,2, Tile(CANNON));
    std::fill_n(e,2, Tile(CLEAR));


    srand(std::chrono::system_clock::now().time_since_epoch().count());

    //shuffle.shrink_to_fit();
    std::random_shuffle(shuffle.begin(),shuffle.end());


    std::vector<int> distanses;
    distanses.reserve(52);

    for(auto p = shuffle.begin();p!=shuffle.end();++p)
    {
        if(p -> getTiletype() == SEA)
            distanses.push_back(std::distance(shuffle.begin(),p));
    }

    cout << " array size : " << shuffle.size() << endl;

    for(int i = 0; i < 13; i++)
    {

        if(shuffle.at(i).getTiletype() != SEA)
        {
            std::swap(shuffle.at(i),shuffle.at(distanses.back()));
            distanses.pop_back();
        }
        else
            distanses.erase(find(distanses.begin(),distanses.end(),i));
    }

    for(int i = 0; i < 169; i+=13)
    {
        if(shuffle.at(i).getTiletype() != SEA)
        {
            std::swap(shuffle.at(i),shuffle.at(distanses.back()));
            distanses.pop_back();
        }
        else
            try {
                auto l = find(distanses.begin(), distanses.end(), i);
                if(l!=distanses.end())
                    distanses.erase(l);
            }catch(const exception& e){}
    }

    for(int i = 12; i < 169; i+=13)
    {
        if(shuffle.at(i).getTiletype() != SEA)
        {
            std::swap(shuffle.at(i),shuffle.at(distanses.back()));
            distanses.pop_back();
        }
        else
            try {
                auto l = find(distanses.begin(), distanses.end(), i);
                if(l!=distanses.end())
                    distanses.erase(l);
            }catch(const exception& e){}
    }

    for(int i = 156; i < 169; i++)
    {

        if(shuffle.at(i).getTiletype() != SEA)
        {
            std::swap(shuffle.at(i),shuffle.at(distanses.back()));
            distanses.pop_back();
        }
        else
            try {
                auto l = find(distanses.begin(), distanses.end(), i);
                if(l!=distanses.end())
                    distanses.erase(l);
            }catch(const exception& e){}
    }

    if (shuffle.at(14).getTiletype() != SEA) {
        std::swap(shuffle.at(14), shuffle.at(distanses.back()));
        distanses.pop_back();
    } else
        try {
            auto l = find(distanses.begin(), distanses.end(), 14);
            if (l != distanses.end())
                distanses.erase(l);
        } catch (const exception &e) {}

    if (shuffle.at(24).getTiletype() != SEA){
        std::swap(shuffle.at(24), shuffle.at(distanses.back()));
        distanses.pop_back();
    }
    else
        try {
            auto l = find(distanses.begin(), distanses.end(), 24);
            if(l!=distanses.end())
                distanses.erase(l);
        }catch(const exception& e){}

    if(shuffle.at(144).getTiletype() != SEA){
        std::swap(shuffle.at(144),shuffle.at(distanses.back()));
        distanses.pop_back();
    }
    else
        try {
            auto l = find(distanses.begin(), distanses.end(), 144);
            if(l!=distanses.end())
                distanses.erase(l);
        }catch(const exception& e){}

    if(shuffle.at(154).getTiletype() != SEA){
        std::swap(shuffle.at(154),shuffle.at(distanses.back()));
        distanses.pop_back();
    }
    else
        try {
            auto l = find(distanses.begin(), distanses.end(), 154);
            if(l!=distanses.end())
                distanses.erase(l);
        }catch(const exception& e){}


    if(distanses.size()) throw "INITITALIZATION GOES WRONG";


    for(int i = 0; i < X_count; i++) {
        for (int j = 0; j < Y_count; j++) {

            std::pair<int, int> pos = std::make_pair(i, j);
            coord_cell[pos] = shuffle.at(13*i + j);

            setDefaultTileSprite(coord_cell[pos]);
            coord_cell[pos].sprite.setScale(scaleX, scaleY);
            coord_cell[pos].sprite.move(i * (offsetX + coord_cell[pos].sprite.getGlobalBounds().width),
                    j * (offsetY + coord_cell[pos].sprite.getGlobalBounds().width));

            coord_cell[pos].coin_number.setPosition(coord_cell[pos].sprite.getPosition());
            coord_cell[pos].galeon_existing.setPosition(coord_cell[pos].sprite.getPosition());
            coord_cell[pos].galeon_existing.move(-30.f,-30.f);
            coord_cell[pos].galeon_existing.move(30.f,-30.f);
            //coord_cell[pos].coin_number.setString(std::to_string(coord_cell[pos].getCoins()));

        }
    }
}


/*SHAKAL::_CELL* SHAKAL::_CELL::getcell(){
    if(!cell)
        return (cell = new _CELL());
    else return cell;
}*/

const std::map<std::pair<int, int>, Tile>& _CELL::getCoordCell() const {
    return coord_cell;
}

Tile& _CELL::operator[](const std::pair<int,int>& pos){
    return coord_cell[pos];
}

std::map<std::pair<int, int>, Tile> &_CELL::getCoordCell() {
    return coord_cell;
}

void _CELL::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.transform *= e.second.trans;
    for(auto& e : getCoordCell())
    {
        target.draw(e.second,states.transform);
    }
}
