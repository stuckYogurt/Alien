//
// Created by sageblatt on 15.04.2022.
//

#include "Level.h"
#include "Game.h"
#include "RandomNumberGenerator.h"
#include <sstream>
#include <iomanip>

using std::string;

Level::Level(std::shared_ptr<RenderWindow> wind, Planets num) {
    window = wind;

    tablice = std::make_unique<Table>();
    for (auto i = 0; i < 5; i++)
        hearts.emplace_back(std::make_unique<Health>(i+1));

    is_lose = false;
    is_win = false;

    waves = {0.1, 0.4, 0.7, 0.99};

    for (auto & it : waves)
        it = last_wave - it * last_wave;

    wave_timer = std::make_unique<Clock>();
    wave_timer->restart();

    font = std::make_unique<Font>();
    font->loadFromFile("./fonts/tab.ttf");

    wave_text = std::make_unique<Text>("Wave", *font);
    wave_text->setFillColor(Color::Yellow);
    wave_text->setCharacterSize(20);
    wave_text->setPosition(window->getSize().x / 2.f - 150, 50);

    win_text = std::make_unique<Text>("Win", *font);
    lose_text = std::make_unique<Text>("Lose", *font);
    win_text->setFillColor(Color::White);
    lose_text->setFillColor(Color::Red);
    win_text->setCharacterSize(20);
    lose_text->setCharacterSize(20);
    win_text->setPosition(100, 700);
    lose_text->setPosition(100, 700);
    win_text->setString("Press TAB to continue");
    lose_text->setString("Press Esc to exit");

    for (auto i = 0; i < 3; i++)
        textures.emplace_back(std::make_unique<Texture>());

    std::string fname, won_name;

    switch (num) {
        case PURPLE:
            fname = "./images/pink_planet.png";
            won_name = "./images/pink_win.png";
            monster_parameters = {200, 100, 2, 50};
            break;
        case FIRE:
            fname = "./images/fire_planet.png";
            won_name = "./images/fire_win.png";
            monster_parameters = {225, 150, 1.5, 75};
            break;
        case ELECTRIC:
            fname = "./images/electric_planet.png";
            won_name = "./images/electric_win.png";
            monster_parameters = {250, 200, 1, 100};
            break;
    }

    textures[0]->loadFromFile(fname);
    textures[1]->loadFromFile("./images/game_over.png");
    textures[2]->loadFromFile(won_name);

    for (auto i = 0; i < 3; i++)
        sprites.emplace_back(std::make_unique<Sprite>(*textures[i]));

    hero = std::make_unique<Player>(400.f,
                                    500,
                                    window->getSize().x,
                                    window->getSize().y,
                                    2000);

    for (auto i = 0; i < 0; i++) {
        monsters.emplace_back(
                std::make_unique<Monster>(monster_parameters.speed,
                                          Game::getInstance()->getRng()->
                                          getRandomInt(0,static_cast<int>(window->getSize().x)),
                                          window->getSize().x, window->getSize().y, monster_parameters.hp, monster_parameters.cooldown));
    }
}

int Level::run() {
    std::stringstream wave_stream;
    wave_stream << std::fixed << std::setprecision(1);

    unsigned current_wave = 0;
    wave_timer->restart();

    hero->resetTimer();
    for (auto & it : monsters)
        it->resetTimer();

    bool hero_shoots;

    while (window->isOpen())
    {
        if (!is_win and !is_lose) {
            Event event;
            while (window->pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window->close();
            }

            if (hero->getHp() <= 0)
                is_lose = true;

            if (last_wave == 0 and monsters.empty())
                is_win = true;

            hero_shoots = hero->keyboard();

            if (hero_shoots) {
                bullets.emplace_back(std::make_unique<Bullet>(hero->getDirectionMove(),
                                                              hero->getPositionX(),
                                                              hero->getPositionY(),
                                                              50.0,
                                                              BULLET_SPEED,
                                                              false));
            }
            hero->move();

            for (auto &it : monsters) {
                it->setDistanceToHero(hero->getPositionX());
                it->move();
                auto fire = it->attack();
                if (fire) {
                    bullets.emplace_back(std::make_unique<Bullet>(it->getDirection(),
                                                                  it->getPositionX(),
                                                                  it->getPositionY(),
                                                                  monster_parameters.damage,
                                                                  BULLET_SPEED,
                                                                  true));
                }
            }

            for (auto &it : monsters) {
                for (auto &it1 : bullets) {
                    if (!it->isRed())
                        it->setSpriteColor(Color::White);

                    if (it->getRect().intersects(it1->getRect()) and !it1->isForHero()) {
                        it->setIsRed(true);
                        if (it->isRed())
                            it->setSpriteColor(Color::Red);

                        it->receiveDamage(it1->getDamage());
                        it1->setLife(false);
                    }
                }
            }

            for (auto &it1 : bullets) {
                if (hero->getRect().intersects(it1->getRect()) and it1->isForHero()) {
                    hero->receiveDamage(it1->getDamage());
                    it1->setLife(false);
                }
            }

            for (auto &it : hearts)
                it->hurt(hero->getHp(), 2000);

            for (auto it = bullets.begin(); it != bullets.end();) {
                (*it)->move();

                if (!(*it)->isLife()) {
                    auto tmpit = it;
                    ++it;
                    bullets.erase(tmpit);
                } else {
                    it++;
                }
            }

            for (auto it = monsters.begin(); it != monsters.end();) {
                if ((*it)->getHp() <= 0) {
                    auto tmpit = it;
                    ++it;
                    monsters.erase(tmpit);
                } else {
                    it++;
                }
            }

            last_wave -= wave_timer->getElapsedTime().asSeconds();
            if (last_wave < 0)
                last_wave = 0;
            if (last_wave < waves[current_wave]) {
                current_wave++;
                spawnMonsters(current_wave);
            }

            wave_timer->restart();
            wave_stream.str(std::string());
            wave_stream.clear();
            wave_stream << last_wave;
            std::string time_until_last = wave_stream.str();
            wave_text->setString(string("              Wave ")
                                     + std::to_string(current_wave) + string("\n")
                                     + string("Time until last wave: ") + time_until_last);

            window->clear();
            window->draw(*sprites[0]);
            window->draw(*wave_text);
            tablice->draw(*window);

            for (auto &it : hearts)
                it->draw(*window);

            hero->draw(*window);
            for (auto &it : monsters)
                it->draw(*window);

            for (auto &bullet : bullets)
                bullet->draw(*window);
        }
        else if (is_win) {
            Event event;
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed)
                    window->close();
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Tab)
                    return 1;
            }
            window->draw(*sprites[2]);
            window->draw(*win_text);
        }
        else if (is_lose) {
            Event event;
            while (window->pollEvent(event)) {
                if (event.type == Event::Closed or (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape))
                    window->close();
            }
            window->draw(*sprites[1]);
            window->draw(*lose_text);
        }

        window->display();
    }
    return 0;
}

void Level::spawnMonsters(unsigned wave) {
    for (int i = 0; i < wave * 2; i++) {
        auto x0 = 0;
        if (Game::getInstance()->getRng()->getRandomInt(0, 1)) {
            x0 = Game::getInstance()->getRng()->getRandomInt(0,
                                                             static_cast<int>(window->getSize().x / 2) - 150);
        } else {
            x0 = Game::getInstance()->
                getRng()->getRandomInt(static_cast<int>(window->getSize().x / 2) + 150,
                                       static_cast<int>(window->getSize().x));
        }
        monsters.emplace_back(
            std::make_unique<Monster>(monster_parameters.speed,
                                      x0,
                                      window->getSize().x,
                                      window->getSize().y,
                                      monster_parameters.hp,
                                      monster_parameters.cooldown));
    }
}
