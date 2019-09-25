//
// Created by User on 25.07.2019.
//

#ifndef SHAKALAKA_PLAYER_H
#define SHAKALAKA_PLAYER_H


#include <SFML/Graphics/Drawable.hpp>
#include <map>
#include <SFML/Graphics/Texture.hpp>
#include <list>

class Pirate;
class Ship;
class player : public sf::Drawable
{
    std::list<Pirate*> pirates;
    int p_id;

    unsigned int score;

    unsigned int rum;
public:

    player();
    player(int id, std::map<std::string, sf::Texture*>& prefub);
    Pirate* getPirateById(int id);

    Pirate* selectPirate(int position);
    Ship* getShip();
    bool isMyPir(int pir_id);

    bool isMyPir(Pirate* pir);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update(float time);

    const std::list<Pirate *> & getPirates() const;

    std::list<Pirate *> & getPirates();
    int getID() const;

    void kill(Pirate *pir);

    void remove(Pirate *pir);
    void born(Pirate *pir);
    void cscore();

    void addrum();
    void userum(Pirate *pirate);
    int getScore() const;

    int getRum() const;
    Ship* ship;

    ~player();

    player& operator=(player& pl);
};


#endif //SHAKALAKA_PLAYER_H
