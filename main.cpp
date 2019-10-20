#include <iostream>
#include <iomanip>

#include <SFML/Graphics.hpp>

#include "Field.h"
#include "SHAKAL.h"
#include <resources.h>
#include <chrono>

using namespace std;

using namespace sf;

/*

int main()
{
    return 0;
}
*/
_textures_ *textures = new _textures_();
_sounds_ *sounds = new _sounds_();

int main()
{

    std::vector<screen*> screens;


    RenderWindow window(VideoMode(1600,900),"Jackal", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    startscreen stscr;
    screens.push_back(&stscr);

    int screen = 0;
    while(screen != -1) {
        screen = screens[screen]->run(window);
        if (screen > 1) {
//            if(screens[1])
//            {
//                delete screens[1];
//            }
            screens[1] = new SHAKAL(screen);
            screen = 1;
        }
    }

    //cout << HORSE_STATE(make_pair(2,2),make_pair(0,1));
    return 0;
}

//*/