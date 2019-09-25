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


int main()
{

    std::vector<screen*> screens;


    RenderWindow window(VideoMode(1600,900),"Jackal", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);


    SHAKAL sh(2);
    screens.push_back(&sh);

    int screen = 0;
    while(screen != -1)
    {
        screen = screens[screen]->run(window);
    }

    cout << std::is_move_assignable<Tile>();


    //cout << HORSE_STATE(make_pair(2,2),make_pair(0,1));
    return 0;
}

//*/