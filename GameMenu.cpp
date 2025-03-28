#include "GameMenu.h"
using std::make_unique;

GameMenu::GameMenu(std::shared_ptr<RenderWindow> wind) {
    window = wind;

    //текстуры
    for (auto i = 0; i < 5; i++)
        textures.emplace_back(make_unique<Texture>());

    textures[0]->loadFromFile("./images/planet_2.png");
    textures[1]->loadFromFile("./images/planet_1.png");
    textures[2]->loadFromFile("./images/planet_3.png");
    textures[3]->loadFromFile("./images/CHOOSE_A_PLANET.png");
    textures[4]->loadFromFile("./images/background_4.png");

    //спрайты
    for (auto i = 0; i < 5; i++)
        sprites.emplace_back(make_unique<Sprite>(*textures[i]));

}

int GameMenu::run() {
    sprites[0]->setPosition(950, 300); // PURPLE 200X200
    sprites[1]->setPosition(500, 500); // FIRE
    sprites[2]->setPosition(200, 250); // ELECTRIC
    sprites[3]->setPosition(300, 60); // CHOOSE_A_PLANET
    sprites[4]->setPosition(0, 0);

    //номер меню и открыто окно или нет
    int menuNum = 0;

    //вызов окна меню
    while (window->isOpen()) {
        sprites[0]->setColor(Color::White);
        sprites[1]->setColor(Color::White);
        sprites[2]->setColor(Color::White);
        menuNum = 0;
        window->clear(Color(129, 181, 221));

        if (IntRect(950, 300, 200, 200).contains(Mouse::getPosition(*window))) {
            sprites[0]->setColor(Color::Blue);
            menuNum = 1;
        } else if (IntRect(500, 500, 200, 200).contains(Mouse::getPosition(*window))) {
            sprites[1]->setColor(Color::Blue);
            menuNum = 2;
        } else if (IntRect(200, 250, 200, 200).contains(Mouse::getPosition(*window))) {
            sprites[2]->setColor(Color::Blue);
            menuNum = 3;
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (menuNum == 1 or menuNum == 2 or menuNum == 3)
                return menuNum;
        }

        window->draw(*sprites[4]);
        window->draw(*sprites[0]);
        window->draw(*sprites[1]);
        window->draw(*sprites[2]);
        window->draw(*sprites[3]);
        window->display();

        Event event;
        while (window->pollEvent(event)) {
            if (event.type == Event::Closed ||
                (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)) {
                    window->close();
            }
        }
    }
    return 1;
}
