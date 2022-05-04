#include "view.hxx"

View::View(Model const& model)
        : model_(model),
          heart("heart_image.jpg"),
          player_one("player_one.png"),
          player_two("player_two.png"),
          round("round.png"),
          prefix_font("sans.ttf", 50),
          prefix(),
          word_sprite(),
          player_one_tag_1("player_one_tag_1.png"),
          player_two_tag_1("player_two_tag_1.png"),
          streak("streak.png"),
          time_remaining("time_remaining.png"),
          time_decreases_1("time_decreases_1.png"),
          time_decreases_2("time_decreases_2.png"),
          powerup("powerups.png"),
          warning("warning.png"),
          p1_streak(),
          p2_streak(),
          timer(),
          player_highlight(Dimensions(223, 200), ge211::Color(197,179,88)),
          round_num(),
          reduce_time(),
          golden_heart("golden_heart.png"),
          swap("swap.png"),
          word_highlight(),
          swap_warning("swap_warning.png"),
          time_drain_1("clock.jpg"),
          time_drain_2("red_arrow.png")


          //round_number(std::to_string(model.round_number))
{
}

void
View::draw(ge211::Sprite_set& set)
{
    //Puts the yellow highlight around the player depending on their turn
    // lines 42-51
    if (model_.player_turn_ == 1){
        set.add_sprite(player_highlight, {165,0}, -1,
                       ge211::geometry::Transform::scale(.65));
    }
    if (model_.player_turn_ == 2){
        set.add_sprite(player_highlight, {2*165 + (3*heart.dimensions().width)
        /10,0}, -1,
                       ge211::geometry::Transform::scale(.65));
    }

    // Lines 54-68 puts static stuff on the screen
    set.add_sprite(player_one_tag_1, {0,0}, 0,
    ge211::geometry::Transform::scale(.65));

    set.add_sprite(powerup, {0,165}, 0,
                   ge211::geometry::Transform::scale(.65));

    set.add_sprite(player_two_tag_1, {640,0}, 0,
                   ge211::geometry::Transform::scale(.65));

    set.add_sprite(powerup, {635,165}, 0,
                   ge211::geometry::Transform::scale(.65));

    set.add_sprite(streak, {20, 60}, 0, ge211::geometry::Transform::scale(.65));
    set.add_sprite(streak, {660, 60}, 0, ge211::geometry::Transform::scale(
            .65));

    // 72-73 displays the time remaining.
    ge211::Text_sprite::Builder timebuilder(prefix_font);
    timebuilder.message(std::to_string(model_.player_time/120));

    //79-86 determines the color of the time.
    if (model_.next_player_drained == 1 and model_.detection == 0){
        timebuilder.color({197,179,88});
    } else {
        timebuilder.color({255,0,0});
    }
    timer.reconfigure(timebuilder);
    set.add_sprite(timer, {365, 330}, 0, ge211::geometry::Transform::scale(
            .90));

    // 89-101 shows the current player's streak.
    ge211::Text_sprite::Builder streakbuilder(prefix_font);
    streakbuilder.message(std::to_string(model_.p1_streak));
    streakbuilder.color({255,0,0});
    p1_streak.reconfigure(streakbuilder);
    set.add_sprite(p1_streak, {65, 95}, 0, ge211::geometry::Transform::scale(
            .90));

    ge211::Text_sprite::Builder streakbuilder2(prefix_font);
    streakbuilder2.message(std::to_string(model_.p2_streak));
    streakbuilder2.color({255,0,0});
    p2_streak.reconfigure(streakbuilder2);
    set.add_sprite(p2_streak, {705, 95}, 0, ge211::geometry::Transform::scale(
            .90));

    // 105- 115 displays the current prefix.
    ge211::Text_sprite::Builder builder(prefix_font);
    builder.message(model_.prefix_);
    if (model_.prefix_swapped == 0){
        builder.color({255,0,0});
    } else {
        builder.color({197,179,88});
    }
    prefix.reconfigure(builder);
    set.add_sprite(prefix, {0, 500
    }, 0);

    //117-126 just displays the typed word.
    if (model_.typed_word.length() > 0){
        ge211::Text_sprite::Builder builder(prefix_font);
        builder.message(model_.typed_word);
        word_sprite.reconfigure(builder);

        int size_x = prefix.dimensions().width;
        set.add_sprite(word_sprite,
                       {size_x, 500});
    }

    int y_offset = 105;
    int x_offset = 165;

    //131-156 deals with displaying hearts.
    for (int i = 0; i < model_.p1_lives - model_.p1_golden_heart; i++) {
        int offset = i*(heart.dimensions().width / 10);
        set.add_sprite(heart, {x_offset + offset, y_offset}, 0,
                       ge211::geometry::Transform::scale(
                               .1));
    }
    if (model_.p1_golden_heart == 1){
        set.add_sprite(golden_heart, {0, 220}, 1,
                       ge211::geometry::Transform::scale(
                               .2));
    }

    for (int i = 0; i < model_.p2_lives - model_.p2_golden_heart; i++) {
        int offset = i*((heart.dimensions().width)/10);
        set.add_sprite(heart,{2*x_offset + (3*heart.dimensions().width)/10 +
                              offset,
                              y_offset}, 0,
                       ge211::geometry::Transform::scale(.1));
    }
    if (model_.p2_golden_heart == 1){
        set.add_sprite(golden_heart, {630, 220}, 1,
                       ge211::geometry::Transform::scale(
                               .2));
    }

// 162-179 shows swap and drain powerups.

    if (model_.p1_swap == 1){
        set.add_sprite(swap, {0, 280}, 1,
                       ge211::geometry::Transform::scale(

                               .3));
    }

    if (model_.p1_drain == 1){
        set.add_sprite(time_drain_1, {0, 370}, 1,
                       ge211::geometry::Transform::scale(

                               .25));
        //int dim = time_drain_1.dimensions().width*.08;
        set.add_sprite(time_drain_2, {60,
                                      370}, 1,
                       ge211::geometry::Transform::scale(
                               .26));
    }

    if (model_.p2_swap == 1){
        set.add_sprite(swap, {630, 280}, 1,
                       ge211::geometry::Transform::scale(
                               .3));
    }

    if (model_.p2_drain == 1){
        set.add_sprite(time_drain_1, {630, 370}, 1,
                       ge211::geometry::Transform::scale(

                               .25));
        //int dim = time_drain_1.dimensions().width*.08;
        set.add_sprite(time_drain_2, {690,
                                      370}, 1,
                       ge211::geometry::Transform::scale(
                               .26));
    }

    set.add_sprite(player_one, {((x_offset + (2*heart.dimensions().width)/10)
    - 58),0},0, ge211::geometry::Transform::scale(.75));

    set.add_sprite(player_two, {((2*x_offset + (3*heart.dimensions().width)/10) +
    (2*heart.dimensions().width)/10) - 60 ,0},0,ge211::geometry::Transform::scale(.75));


    set.add_sprite(round, {(x_offset + (2*heart.dimensions().width)/10) + 65,
                           y_offset - 80}, 0, ge211::geometry::Transform::scale(
                                   .75));

// 216- 222 displays round number.
    ge211::Text_sprite::Builder roundbuilder(prefix_font);
    roundbuilder.message(std::to_string(model_.round_num));
    //std::cout << model_.round_num;
    roundbuilder.color({255,0,0});
    round_num.reconfigure(roundbuilder);
    set.add_sprite(round_num, {380, y_offset-25}, 0,
                   ge211::geometry::Transform::scale(
            1));

    set.add_sprite(time_remaining, {(x_offset + (2*heart.dimensions().width)
    /10) - 30 , 280}, 0, ge211::geometry::Transform::scale(.75));

    set.add_sprite(time_decreases_1, {(x_offset + (2*heart.dimensions().width)
                                                /10) - 30 , 160}, 0,
                   ge211::geometry::Transform::scale(.75));
    set.add_sprite(time_decreases_2, {(x_offset + (2*heart.dimensions().width)
                                                  /10) - 30 , 220}, 0,
                   ge211::geometry::Transform::scale(.75));

    //235-242 displays reduce time counter.
    ge211::Text_sprite::Builder reducebuilder(prefix_font);
    reducebuilder.message(std::to_string(model_.reduce_time_counter));
    reducebuilder.color({255,0,0});
    reduce_time.reconfigure(reducebuilder);
    set.add_sprite(reduce_time, {(x_offset + (2*heart.dimensions().width)
                                                  /10) + 190, 210}, 0,
                   ge211::geometry::Transform::scale(1));


    if (model_.flash and model_.warning){
        set.add_sprite(warning, {(x_offset + (2*heart.dimensions().width)
                                             /10) - 30 , 400}, 0,
                       ge211::geometry::Transform::scale(.65));
    }

    if (model_.prefix_swapped and model_.flash){
        set.add_sprite(swap_warning, {(x_offset + (2*heart.dimensions().width)
                                             /10) + 45, 455}, 0,
                       ge211::geometry::Transform::scale(.65));
    }
}

