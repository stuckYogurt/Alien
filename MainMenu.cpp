#include "MainMenu.h"

MainMenu::MainMenu(std::shared_ptr<RenderWindow> wind) {
    window = wind;

    //текстуры
    for (auto i = 0; i < 4; i++)
        textures.emplace_back(make_unique<Texture>());

    textures[0]->loadFromFile("./images/CONTINUE.png");
    textures[1]->loadFromFile("./images/EXIT.png");
    textures[2]->loadFromFile("./images/NEW_GAME.png");
    textures[3]->loadFromFile("./images/background_4.png");

    //спрайты
    for (auto i = 0; i < 4; i++)
        sprites.emplace_back(make_unique<Sprite>(*textures[i]));
}

int MainMenu::run() {
    sprites[0]->setPosition(350, 200);
    sprites[1]->setPosition(350, 400);
    sprites[2]->setPosition(350, 200);
    sprites[3]->setPosition(0, 0);

    //номер меню
    int menuNum = 0;

    //вызов окна меню
    while (window->isOpen()) {
        sprites[0]->setColor(Color::White);
        sprites[1]->setColor(Color::White);
        sprites[2]->setColor(Color::White);
        menuNum = 0;
        window->clear(Color(129, 181, 221));

         if (IntRect(500, 400, 300, 100).contains(Mouse::getPosition(*window))) {
             sprites[1]->setColor(Color::Red);
             menuNum = 1; //кнопка exit(убрали continue)
         }
         if (IntRect(450, 200, 600, 100).contains(Mouse::getPosition(*window))) {
             sprites[2]->setColor(Color::Red);
             menuNum = 2; //кнопка new_game
         }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (menuNum == 1)
                window->close(); //если нажали первую кнопку, то выходим из меню
        }

        if (Mouse::isButtonPressed(Mouse::Left)) {
            if (menuNum == 2) //если нажали кнопку new_game, то перешли в GameMenu
                return 0;
        }

        window->draw(*sprites[3]);
        for (auto i = 1; i < 3; i++)
            window->draw(*sprites[i]);
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
