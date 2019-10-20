//
// Created by User on 17.08.2019.
//

#ifndef SHAKALAKA_RESOURCES_H
#define SHAKALAKA_RESOURCES_H
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/Music.hpp>
#include <utility>
#include <dirent.h>
#include <chrono>
#include <sstream>


static std::map<std::string,sf::Texture*>& loadTfrom(const char* path, const char* format, std::map<std::string,sf::Texture*>& buffer)
{
    using namespace sf;
    using namespace std;
    DIR *dir;
    struct dirent* ent;

    if((dir = opendir(path)) != nullptr)
    {
        while((ent = readdir(dir))!= nullptr)
        {
            std::string str(ent->d_name);
            if(str.rfind(format) == std::string::npos) continue;

            Texture *t = new Texture();
            if(!t->loadFromFile(string(path).append(ent->d_name)))
            {
                delete t;
                continue;
            }

            buffer[ent->d_name] = t;
        }

        closedir(dir);
    }else
    {
        perror("Cannot open directory ./textures");
        return buffer;
    }

    return buffer;
}

struct _fonts_{

sf::Font coins_;

    _fonts_()
    {
      coins_.loadFromFile("font.ttf");
    };
};

static _fonts_ *fonts = new _fonts_();

struct _textures_{

    _textures_()
    {    };

    void initSH(){
        //textures_.clear();
        loadTfrom("./textures/", ".png", textures_);
    };

    void initStScr(){
        //textures_.clear();
        sf::Texture *c = new sf::Texture();
            c->loadFromFile("./textures/paper.png");
        textures_["paper.png"] = c;
        c = new sf::Texture();
            c->loadFromFile("./textures/icon_map.png");
        textures_["icon_map.png"] = c;
        c = new sf::Texture();
            c->loadFromFile("./textures/background.png");
        textures_["background.png"] = c;
    };

    std::map<std::string, sf::Texture *> textures_;

    ~_textures_(){
        std::cerr << "texutres somehow destroyed!\n";
        for(auto &e : textures_)
            if(e.second)
                delete e.second;
    };
};

extern _textures_ *textures;


struct _sounds_{
    sf::SoundBuffer buff[10];
    sf::Music maintheme, shakal_theme;

    _sounds_(){

        if(!buff[0].loadFromFile("sounds/yes.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("yes", &buff[0]));
        if(!buff[1].loadFromFile("sounds/no.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("no", &buff[1]));
        if(!buff[2].loadFromFile("sounds/catch.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("catch", &buff[2]));
        if(!buff[3].loadFromFile("sounds/coinget.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("coinget", &buff[3]));
        if(!buff[4].loadFromFile("sounds/move.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("move", &buff[4]));
        if(!buff[5].loadFromFile("sounds/mydear.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("mydear", &buff[5]));
        if(!buff[6].loadFromFile("sounds/win.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("win", &buff[6]));
        if(!buff[7].loadFromFile("sounds/ok.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("ok", &buff[7]));
        if(!buff[8].loadFromFile("sounds/yohohoo.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("yohoho", &buff[8]));
        if(!buff[9].loadFromFile("sounds/gachi.ogg"))
            std::cerr << "AUDIO ERR\n";
        else sounds_.insert(std::make_pair("gachi", &buff[9]));

        if(!maintheme.openFromFile("sounds/menutheme.ogg"))
            std::cerr<< "Missed maintheme\n";
        if(!shakal_theme.openFromFile("sounds/Tabletop_Battles.ogg"))
            std::cerr << "Missed shakaltheme\n";
        maintheme.setLoop(true);
        shakal_theme.setLoop(true);

    };


    std::map<std::string, sf::SoundBuffer *> sounds_;



    void playmenu(){
        shakal_theme.stop();
        maintheme.setVolume(1.f);
        maintheme.play();
    };

    void playshakal(){
        maintheme.stop();
        shakal_theme.setVolume(1.f);
        shakal_theme.play();
    };

    void play(std::string name){
        if(ass.getStatus() == sf::Sound::Playing)
            return;

        if(name == "move")
            ass.setBuffer(buff[4]);
        else if(name == "catch")
            ass.setBuffer(buff[2]);
        else if(name == "yes")
            ass.setBuffer(buff[0]);
        else if(name == "no")
            ass.setBuffer(buff[1]);
        else if(name == "mydear")
            ass.setBuffer(buff[5]);
        else if(name == "gachi")
            ass.setBuffer(buff[9]);
        else if(name == "yohoho")
            ass.setBuffer(buff[8]);
        else if(name == "win")
            ass.setBuffer(buff[6]);
        else if(name == "coinget")
            ass.setBuffer(buff[3]);
        else if(name == "ok")
            ass.setBuffer(buff[7]);

        ass.setVolume(5.f);
        ass.play();
    };

    ~_sounds_(){
        ass.stop();
        maintheme.stop();
        shakal_theme.stop();
        delete &buff;
    };
    sf::Sound ass;
};

extern _sounds_ *sounds;

enum Action2Log{
    MOVED,
    RETURNED,
    PICK,
    DROP,
    NEWTURN
};

static std::string A2T()
{
    std::stringstream stroke;

    auto timestamp = time(nullptr);

    tm* ltm = localtime(&timestamp);
    stroke << "[" << ltm->tm_hour << ":"
     << ltm->tm_min << ":" << ltm->tm_sec << "]";

    return stroke.str();
}


#endif //SHAKALAKA_RESOURCES_H
