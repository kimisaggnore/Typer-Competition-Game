#include "controller.hxx"
#include <stdexcept>
static std::vector<std::string>
load_dictionary(std::string const& filename)
{
    std::ifstream dictionary = ge211::open_resource_file(filename);
    std::vector<std::string> result;
    std::string buffer;

    while (std::getline(dictionary, buffer)) {
        result.push_back(buffer);
    }

    if (dictionary.bad()) {
        throw std::runtime_error("could not read dictionary: " + filename);
    }

    return result;
}

Controller::Controller(std::vector<std::string> const& words)
        : model_(words),
          view_(model_)
{ }

Controller::Controller(std::string const& filename)
        : Controller(load_dictionary(filename))
{ }



void
Controller::draw(ge211::Sprite_set& set)
{
    model_.on_frame();
    if (model_.game_time % 60 == 0){
        model_.flashing();
    }
    if (model_.player_time <= 0){
        model_.time_is_up();
    }
    view_.draw(set);
}

void
Controller::on_key(ge211::Key key) {
    if (key.is_textual()){
        model_.push_letter(key.as_text());
    }
    if (key == ge211::Key::control()){
        model_.erase_word();
    }
    if (key == ge211::Key::right()){
        model_.make_move();
    }
    if (key == ge211::Key::left()){
        int cur_turn = model_.player_turn_;
        if (cur_turn == 1){
            if (model_.p1_swap == 1){
                model_.swap_prefix();
                model_.p1_swap = 0;
            }
        }
        if (cur_turn == 2){
            if (model_.p2_swap == 1){
                model_.swap_prefix();
                model_.p2_swap = 0;
            }
        }

    }
    if (key == ge211::Key::down()){
        int cur_turn = model_.player_turn_;
        if (cur_turn == 1){
            if (model_.p1_drain == 1){
                model_.drain_next_player();
                model_.p1_drain = 0;
            }
        }
        if (cur_turn == 2){
            if (model_.p2_drain == 1){
                model_.drain_next_player();
                model_.p2_drain = 0;
            }
        }
    }
}