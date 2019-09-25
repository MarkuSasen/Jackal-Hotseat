//
// Created by User on 17.08.2019.
//

#ifndef SHAKALAKA_RESOURCES_H
#define SHAKALAKA_RESOURCES_H
#include <SFML/Graphics/Font.hpp>

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
