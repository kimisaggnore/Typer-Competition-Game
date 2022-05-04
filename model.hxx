#pragma once

#include <ge211.hxx>
#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <random>
#include <chrono>
#include <vector>

class Model
{
public:
    explicit Model(std::vector<std::string> const& words);
    std::string
    set_new_prefix(std::vector<std::string>, std::string);

    std::vector<std::string>
    construct_dictionary(std::vector<std::string>, std::string, bool);

    bool
    valid_prefix(std::vector<std::string> , std::string, bool);

    bool
    valid_input(std::string word, std::vector<std::string> dict, bool);

    void
    on_frame();

    void
    push_letter(std::string);

    void
    erase_word();

    void
    make_move();

    int
    other_player(int, bool);

    bool
    in_used_words(std::string);

    void
    flashing();

    void
    add_to_streak();

    void
    reset_streak();

    void
    time_is_up();

    void
    update_success();

    int
    check_success();

    int
    calculate_player_time();

    void
    swap_prefix();

    void
    drain_next_player();

    std::vector<std::string> dictionary_;
    std::vector<std::string> used_words_;
    std::string prefix_;
    std::vector<std::string> typed_word_arr_;
    std::string typed_word;
    int max_word_length;
    int player_turn_;
    std::vector<std::string> cur_dict;
    int p1_lives;
    int p2_lives;
    int round_num;
    bool game_started;
    std::mt19937 mt;
    bool warning;
    int game_time;
    bool flash;
    int p1_streak;
    int p2_streak;
    int counter;
    int player_time;
    std::vector<int> success;
    int reduce_time_counter;
    int time_decreases_count;
    int init_game_time;
    int p1_golden_heart;
    int p2_golden_heart;
    int p1_swap;
    int p2_swap;
    int prefix_swapped;
    int p1_drain;
    int p2_drain;
    int next_player_drained;
    int detection;
    int next_player_drained2;
    int active_drain;
    int max_prefix_size;
    int power_word_len;
    int streak_num_powerup;



};


