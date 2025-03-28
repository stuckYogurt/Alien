//
// Created by sageblatt on 19.04.2022.
//
#include "Health.h"

Table::Table() {
    texture_tab = std::make_unique<Texture>();
    sprite_tab = std::make_unique<Sprite>();

    texture_tab->loadFromFile("./images/tablice.png");
    sprite_tab->setTexture(*texture_tab);
    sprite_tab->setPosition(50, -83);
    sprite_tab->setScale(0.5, 0.8);
}

void Table::draw(RenderWindow& window) {
    window.draw(*sprite_tab);
}

Health::Health(int i) {
    heart_vec.push_back({0, 125, 1, 115});
    heart_vec.push_back({1000 - 125, 125, 1, 115});

    count = i;

    texture_heart = std::make_unique<Texture>();
    sprite_heart = std::make_unique<Sprite>();

    texture_heart->loadFromFile("../images/health.png");
    sprite_heart->setTexture(*texture_heart);

    sprite_heart->setPosition(0.4 * 129 * count + 55, 25);
    sprite_heart->setScale(0.4, 0.5);
}

void Health::draw(RenderWindow& window) {
    window.draw(*sprite_heart);
}

void Health::hurt(double hp_, double maxHp_) {
    auto hp = (int)(hp_ / maxHp_ * 5);
    auto maxHp = 5;
    if  (count <= hp) {
        sprite_heart->setTextureRect(IntRect(heart_vec[0].x0, heart_vec[0].y0, heart_vec[0].lenX, heart_vec[0].lenY));
        return;
    }
    else if (count <= maxHp) {
        sprite_heart->setTextureRect(IntRect(heart_vec[1].x0, heart_vec[1].y0, heart_vec[1].lenX, heart_vec[1].lenY));
        return;
    }
}