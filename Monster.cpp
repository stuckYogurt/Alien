#include "Monster.h"


Monster::Monster(float sp_x, float x0, int wind_w, int wind_h, double health, float cd):
COOLDOWN(cd) {
    timer = std::make_unique<Clock>();
    timer->restart();
    dt = 0;

    is_firing = false;
    distance_to_hero = 0;
    direction = 0;

    hp = health;

    move_vec.push_back({0, 524, 65, 60 });
    move_vec.push_back({78, 528, 65, 60 });

    attack_vec.push_back({0, 0, 83, 59 });
    attack_vec.push_back({0, 315, 83, 60 });
    attack_vec.push_back({0, 455, 83, 60 });
    attack_vec.push_back({82, 455, 83, 60 });
    attack_vec.push_back({0, 455, 83, 60 });
    attack_vec.push_back({0, 315, 83, 60 });
    attack_vec.push_back({0, 0, 83, 59 });

    fall_vec.push_back({0, 139, 65, 65 });

    window_width = wind_w;
    window_height = wind_h;

    x = x0;

    y = 0;

    speed_x = sp_x;
    speed_y = 0;

    on_ground = false;

    texture.loadFromFile("./images/ALIEN.png");
    sprite.setTexture(texture);

    sprite.setPosition(x, y);

    current_frame = 0;
    current_frame_2 = 0;
}

void Monster::move() {
    dt = timer->getElapsedTime().asSeconds();
    timer->restart();

    red_cooldown -= dt;

    if (red_cooldown <= 0) {
        red_cooldown = 0.5;
        is_red = false;
    }

    if (sprite.getPosition().y >= (float)(window_height - fall_vec[0].height)) {
        on_ground = true;
        speed_y = 0;
    }

    if (!on_ground) {
        current_frame += FRAME_RATIO * dt;

        if (current_frame > 10)
            current_frame = 0;

        on_ground = false;
        sprite.setTextureRect(IntRect(fall_vec[0].x,
                                      fall_vec[0].y,
                                      fall_vec[0].width,
                                      fall_vec[0].height));
        sprite.move(0, -speed_y * dt + g_accel * dt * dt / 2);
        speed_y = speed_y - g_accel * dt;
    }



    if (on_ground)
        return;
}

bool Monster::attack() {
    cooldown_left -= dt;
    if (cooldown_left <= 0 and on_ground) {
        cooldown_left = COOLDOWN;
        is_firing = true;
        return true;
    }

    if (current_frame >= 5) {
        current_frame = 0;
        is_firing = false;
    }

    if (distance_to_hero >= 0)
        direction = 1;
    else
        direction = -1;

    if (on_ground && std::abs(distance_to_hero) <= 300) {
        if (distance_to_hero >= 0) {
            if (!is_firing) {
                sprite.setTextureRect(IntRect(attack_vec[0].x,
                                              attack_vec[0].y,
                                              attack_vec[0].width,
                                              attack_vec[0].height));
                return false;
            }

            sprite.setTextureRect(IntRect(attack_vec[(int)current_frame].x,
                                          attack_vec[(int)current_frame].y,
                                          attack_vec[(int)current_frame].width,
                                          attack_vec[(int)current_frame].height));
            current_frame += dt * FRAME_RATIO;
        } else {
            if (!is_firing)
            {
                sprite.setTextureRect(IntRect(attack_vec[0].width,
                                              attack_vec[0].y,
                                              -attack_vec[0].width,
                                              attack_vec[0].height));
                return false;
            }
            sprite.setTextureRect(IntRect(attack_vec[(int)current_frame].x + attack_vec[(int)current_frame].width,
                                          attack_vec[(int)current_frame].y,
                                          -attack_vec[(int)current_frame].width,
                                          attack_vec[(int)current_frame].height));
            current_frame += dt * FRAME_RATIO;
        }
    }
    else if (on_ground && distance_to_hero > 300) {
        current_frame += dt * FRAME_RATIO_1;
        int j = (int)(current_frame / 3);
        sprite.setTextureRect(IntRect(move_vec[j].x,
                                      move_vec[j].y,
                                      move_vec[j].width,
                                      move_vec[j].height));
        sprite.move(speed_x * dt, 0);
    }
    else if (on_ground && distance_to_hero < -300) {
        current_frame += dt * FRAME_RATIO_1;
        int j = (int)(current_frame / 3);
        sprite.setTextureRect(IntRect(move_vec[j].x + move_vec[j].width,
                                      move_vec[j].y,
                                      -move_vec[j].width,
                                      move_vec[j].height));
        sprite.move(-speed_x * dt, 0);
    }
    return false;
}

void Monster::setDistanceToHero(float hero_pos) {
    distance_to_hero = hero_pos - sprite.getPosition().x;
}

float Monster::getPositionX() {
    return sprite.getPosition().x;
}

float Monster::getPositionY() {
    return sprite.getPosition().y;
}

int Monster::getDirection() const {
    return direction;
}

bool Monster::isRed() const {
    return is_red;
}

void Monster::setIsRed(bool red) {
    is_red = red;
}
