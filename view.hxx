#pragma once

#include "model.hxx"

class View
{
public:
    using Dimensions = ge211::Dims<int>;
    explicit View(Model const& model);

    void draw(ge211::Sprite_set& set);

private:
    Model const& model_;
    ge211::Image_sprite const heart;
    ge211::Image_sprite const player_one;
    ge211::Image_sprite const player_two;
    ge211::Image_sprite const round;
    ge211::Font prefix_font;
    ge211::Text_sprite prefix;
    ge211::Text_sprite word_sprite;
    ge211::Image_sprite const player_one_tag_1;
    ge211::Image_sprite const player_two_tag_1;
    ge211::Image_sprite const streak;
    ge211::Image_sprite const time_remaining;
    ge211::Image_sprite const time_decreases_1;
    ge211::Image_sprite const time_decreases_2;
    ge211::Image_sprite const powerup;
    ge211::Image_sprite const warning;
    ge211::Text_sprite p1_streak;
    ge211::Text_sprite p2_streak;
    ge211::Text_sprite timer;
    ge211::Rectangle_sprite const player_highlight;
    ge211::Text_sprite round_num;
    ge211::Text_sprite reduce_time;
    ge211::Image_sprite golden_heart;
    ge211::Image_sprite swap;
    ge211::Text_sprite word_highlight;
    ge211::Image_sprite swap_warning;
    ge211::Image_sprite time_drain_1;
    ge211::Image_sprite time_drain_2;
    //ge211::Text_sprite const round_number;
};
