//
// Created by User on 17.08.2019.
//

#ifndef SHAKALAKA_RESOURCES_H
#define SHAKALAKA_RESOURCES_H
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>
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
    {
        loadTfrom("./textures/", ".png", textures_);
    };

    std::map<std::string, sf::Texture *> textures_;
};

static _textures_ *textures = new _textures_();


struct _sounds_{
    sf::SoundBuffer buff[10];

    _sounds_(){

#define LOADFROM(i,PATH) buff[i].loadFromFile(std::string("sounds/").append(#PATH))
        LOADFROM(0,aiyapopalsya.ogg); LOADFROM(1,dakapitan.ogg); LOADFROM(2,kakskazhite.ogg);
        LOADFROM(3,mistaliblizhekpobede.ogg); LOADFROM(4,moyaprelest.ogg);
        LOADFROM(5,poboremsya.ogg); LOADFROM(6,tistalpiratom.ogg); LOADFROM(7,vidvigayus.ogg);
        LOADFROM(8,yanesdelayueto.ogg); LOADFROM(9,yohohoo.ogg);

        for(int i = 0 ; i < 10; i++)
        {
            sounds_.insert(std::make_pair(static_cast<Sounds>(i),&buff[i]));
        }
#undef LOADFROM
    };

    enum Sounds {
            POPALSYA = 0,
                DA = 1,OK = 2,OCHKI = 3,COIN,FIGHT,WIN,MOVE = 7,NEMOGU,HOHOJAJAOLALA
    };

    std::map<Sounds, sf::SoundBuffer *> sounds_;

    void play(Sounds e){
        sf::Sound s;
        s.setBuffer(*sounds_.at(e));
        s.setVolume(50.f);
        s.play();
    };

    ~_sounds_(){
        delete &buff;
    }
};

static _sounds_ *sounds = new _sounds_();

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
