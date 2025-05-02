//
// Created by sageblatt on 01.04.2022.
//
#ifndef ALIEN_MENU_H
#define ALIEN_MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>


class Menu {
protected:
    std::shared_ptr<sf::RenderWindow> window;
    std::vector <std::unique_ptr <sf::Texture>> textures;
    std::vector <std::unique_ptr <sf::Sprite>> sprites;

    unsigned short fps;
public:
    Menu();
    virtual int run() = 0;
    virtual ~Menu();
};


#endif //ALIEN_MENU_H